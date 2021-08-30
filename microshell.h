#ifndef __MICROSHELL_H__
#define __MICROSHELL_H__

/* Header(s) */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/* Define(s) */

#define SIDE_OUT    0
#define SIDE_IN     1

#define STDIN       0
#define STDOUT      1
#define STDERR      2

#define TYPE_END    0
#define TYPE_PIPE   1
#define TYPE_BREAK  2

/* Structure(s) */

typedef struct  s_stack {
    char            **args;
    char            type;
    unsigned int    size;

    struct s_stack  *previous;
    struct s_stack  *next;
}               t_stack;

#endif