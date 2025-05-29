#ifndef PARSEARG_H
#define PARSEARG_H

#include "utils.h"

#define PROGRAM_NAME "./eleg"
#define FILE_LIST (argv + 1)
#define FILE_COUNT (argc - 1)
#define TARGET_WORD (argv[0])

void UsageMessage(void);
void VersionMessage(void);
char **ParseArgList(int *argc, char **argv);

extern int _g_malloc_used;

#endif // PARSEARG_H
