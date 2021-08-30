#ifndef __MICROSHELL_H__
#define __MICROSHELL_H__

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct  s_args {
    char    **args;
    char    type;

    struct s_args   *previous;
    struct s_args   *next;
}               t_args;

#endif