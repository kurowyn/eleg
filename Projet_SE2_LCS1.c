#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE_LEN 1024

enum {
    TROUVE = 0,
    NON_TROUVE = 1,
    FICHIER_INEXISTANT = 2,
    IMPOSSIBLE_OUVRIR = 3
};

int search_file(const char *filename, const char *search_str) {
    FILE *f = fopen(filename, "r");
    if (!f)
        return FICHIER_INEXISTANT;

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, f)) {
        if (strstr(line, search_str)) {
            fclose(f);
            return TROUVE;
        }
    }
    fclose(f);
    return NON_TROUVE;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s MOT FICHIER [FICHIER ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *mot = argv[1];
    int nb = argc - 2;

    struct {
        const char *name;
        pid_t pid;
        int status;
    } *files = malloc(nb * sizeof *files);
    if (!files) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nb; i++) {
        files[i].name = argv[i + 2];
        
        if (access(files[i].name, F_OK) == -1) {
            files[i].status = FICHIER_INEXISTANT;
            files[i].pid = 0;
            continue;
        }
        FILE *t = fopen(files[i].name, "r");
        if (!t) {
            files[i].status = IMPOSSIBLE_OUVRIR;
            files[i].pid = 0;
            continue;
        }
        fclose(t);

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            exit(search_file(files[i].name, mot));
        }
        files[i].pid = pid;
    }

    for (int i = 0; i < nb; i++) {
        if (files[i].pid != 0) {
            int wstatus;
            waitpid(files[i].pid, &wstatus, 0);
            if (WIFEXITED(wstatus))
                files[i].status = WEXITSTATUS(wstatus);
            else
                files[i].status = -1;
        }

        switch (files[i].status) {
            case TROUVE:
                printf("%s : '%s' Mot trouvé.\n", files[i].name, mot);
                break;
            case NON_TROUVE:
                printf("%s : '%s' Mot non trouvé.\n", files[i].name, mot);
                break;
            case FICHIER_INEXISTANT:
                printf("%s : Le fichier est introuvable.\n", files[i].name);
                break;
            case IMPOSSIBLE_OUVRIR:
                printf("%s : Le fichier ne peut pas être ouvert.\n", files[i].name);
                break;
            default:
                printf("%s : erreur inconnue (code %d).\n", files[i].name, files[i].status);
        }
    }

    free(files);
    return EXIT_SUCCESS;
}

