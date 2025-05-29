#include "parsearg.h"

int _g_malloc_used = 0;

char **
ParseArgList(int *argc, char **argv){
    int parsed_argc = *argc;
    char **parsed_argv = argv,
         *option_arg,
         target_arg[MAX_BUFFER_LEN],
         buffer[MAX_BUFFER_LEN];

    if (parsed_argc > 1) {
        // Check if something like this happens:
        // ./eleg a -b -c -d
        for (ssize_t i = 1; i < parsed_argc; ++i) {
            if (parsed_argv[i][0] == '-' && i != 1) {
                UsageMessage();
            } 
        }

        // Get the option argument.
        option_arg = parsed_argv[1];

        if (STR_EQ(option_arg, "-c")) {
            if (parsed_argc < 4) { 
                UsageMessage();
            // ./eleg -c A B ... 
            } else {
                parsed_argc = parsed_argc - 2;

                *argc = parsed_argc;
                return parsed_argv + 2;
            }
        } else if (STR_EQ(option_arg, "-i")) {
            // ./eleg -i A B ...
            if (parsed_argc >=  4) {
                parsed_argc = parsed_argc - 2;

                *argc = parsed_argc;
                return parsed_argv + 2;
            } else {
                fprintf(stdout, "Press enter on an empty input to end input.\n");

                // We allocate memory for our new arg list.
                parsed_argv = malloc(sizeof (char *));
                ERR_HANDLE(parsed_argv == NULL,
                           "ERROR: could not allocate memory, errno: %d\n",
                           exit(errno),
                           errno);

                _g_malloc_used = 1;
                
                // ./eleg -i A (only)
                if (parsed_argc == 3) {
                    strcpy(target_arg, argv[2]);
                // ./eleg -i (only)
                } else {
                    STRING_INPUT("Target word: ", target_arg, 0, 0);
                    // fgets could fail.
                    if (!target_arg || STR_EQ(target_arg, "")) {
                        UsageMessage();
                    }
                }
                parsed_argv[0] = malloc(strlen(target_arg));
                ERR_HANDLE(parsed_argv == NULL,
                           "ERROR: could not allocate memory, errno: %d\n",
                           exit(errno),
                           errno);
                strcpy(parsed_argv[0], target_arg);
                // We appeneded the target word to our newly created list.
                // We update its length accordingly.
                parsed_argc = 1;

                while (1) {
                    STRING_INPUT("File %d: ", buffer, 0, parsed_argc);

                    if (buffer != NULL && !STR_EQ(buffer, "")) {
                        parsed_argv = realloc(parsed_argv,
                                              ++parsed_argc * sizeof (char *));
                        
                        ERR_HANDLE(parsed_argv == NULL,
                                   "ERROR: could not allocate memory, errno: %d\n",
                                   exit(errno),
                                   errno);

                        parsed_argv[parsed_argc - 1] = malloc(strlen(buffer));

                        ERR_HANDLE(parsed_argv[parsed_argc - 1] == NULL,
                                   "ERROR: could not allocate memory, errno: %d\n",
                                   exit(errno),
                                   errno);

                        strcpy(parsed_argv[parsed_argc - 1], buffer);
                    } else {
                        break;
                    }
                }

                if (parsed_argc >= 2) {
                    *argc = parsed_argc;
                    return parsed_argv;
                } else {
                    UsageMessage();
                }
            }
        } else if (STR_EQ(option_arg, "-h")) {
            UsageMessage();
        } else if (STR_EQ(option_arg, "-v") || STR_EQ(option_arg, "--version")) {
            VersionMessage();
        } else if (option_arg[0] != '-') {
            if (parsed_argc < 3) {
                UsageMessage();
            // ./eleg A B ...
            } else {
                parsed_argc = parsed_argc - 1;

                *argc = parsed_argc;
                return argv + 1;
            }
        } else {
            UsageMessage();
        }
    } else {
        UsageMessage();
    }
}

void
UsageMessage(void){
    fprintf(stderr, "usage: %s [OPTION] (word) (FILE(s))\n" 
                    "OPTION may be one of the following: \n"
                    "-c: command-line mode (default)\n"
                    "-h: show this help and exit\n"
                    "-i: interactive session\n"
                    "-v | --version: show version and exit\n",
                    PROGRAM_NAME);
    exit(EXIT_FAILURE);
}

void
VersionMessage(void){
    fprintf(stdout, "eleg 0.3 - an elementary implementation of grep in the C "
                    "programming language.\n"
                    "Licensed under GPLv2 or later.\n");
    exit(EXIT_SUCCESS);
}
