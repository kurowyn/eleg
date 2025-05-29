#ifndef PARSEARG_H
#define PARSEARG_H

#include "utils.h"

void UsageMessage(void);
void VersionMessage(void);
char **ParseArgList(int *argc, char **argv);

extern int _g_malloc_used;

#endif // PARSEARG_H
