#include "microshell.h"

int parse(t_args **args, char *av) {
    
    
    
    return (0);
}

int main(int ac, char **av) {
    t_args  *args;

    args = NULL;
    for (unsigned int i = 1; i < ac; i++) {
        parse(&args, av[i]);
    }

    return (0);
}