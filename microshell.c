#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Basique fonction pour calculer la taille d'une chaine de caractère. */
static size_t ft_strlen(char *s) {
	size_t i = 0;
	
	while (s && s[i]) i++;
	return (i);
}

static void _err(char *err, char *path) {
	/* Affiche la variable err sur la sortie d'erreur. */
	write(2, err, ft_strlen(err));

	/* Affiche la variable path sur la sortie d'erreur si elle n'est pas égal à NULL. */
	if (path) {
		write(2, path, ft_strlen(path));
		write(2, "\n", 1);
	}
	/* Quitte le programme en cours avec une erreur. */
	exit(1);
}

static char **_subav(char *argv[], int start, int end)
{
	char **res;

	/* Alloue de la mémoire pour copié les arguments de argv de start à end dans res. */
	if (!(res = malloc(sizeof(char *) * (end - start + 1))))
		return (NULL);

	/* Copie les arguments de argv de start à end dans res. */
	int i = 0;
	while (start < end)
		res[i++] = argv[start++];
	res[i] = NULL;

	return (res);
}

int main(int argc, char *argv[], char *env[]) {
	/* Initialise toutes les variables. */
	char **av;
	pid_t pid;
	int i = 1;
	int pos_semicolon, start, end, fd[2], fd_in;

	/* Boucle tant que i est inferieur au nombre d'argument. */
	while (i < argc) {
		/* Initialise semicolon, start, end à la valuer de i(0). */
		pos_semicolon = start = end = i;

		/* Boucle sur tous les arguments tant que la position acutelle de pos_semicolon est inferieur au nombre d'argument ou que argv[pos_semicolon] est égal a ';'. */
		while (pos_semicolon < argc && strcmp(argv[pos_semicolon], ";"))
			pos_semicolon++;

		/* Met la variable fd_in à 0. */
		fd_in = 0;

		/* Boucle tant que la position start est inferieur a la position pos_semicolon. */
		while (start < pos_semicolon) {
			end = start;

			/* Boucle tant que la position start est inferieur a la position pos_semicolon ou que argv[end] n'est pas égal a un pipe. */
			while (end < pos_semicolon && strcmp(argv[end], "|"))
				end++;

			/* Copie la liste des arguments de argv de start à end dans une nouvelle variable av. */
			av = _subav(argv, start, end);
			if (av == NULL || pipe(fd) == -1)
				_err("error: fatal\n", NULL);

			/* Crée un processus enfant pour faire un appele système. */
			if ((pid = fork()) == -1) /* Error lors de la création du processus. */
				_err("error: fatal\n", NULL);
			else if (pid == 0) { /* Processus enfant */
				/* Si la duplication de fd_in dans l'entrée standart ne fonctionne pas ou que la position du dernier argument est inferieur a la position de la semicolon et que la duplication du fd[1] dans la sortie standart échoue alors une erreur est renvoyé. */
				if (dup2(fd_in, 0) == -1 || (end < pos_semicolon && dup2(fd[1], 1) == -1))
					_err("error: fatal\n", NULL);

				/* Fermeture de tous les fds utilisé pour éviter toute fuite de file descriptor. */
				close(fd_in);
				close(fd[0]);
				close(fd[1]);
				
				/* Comparaison de l'argument 0 (la commande) pour savoir si il s'agit de la commande 'cd'. */
				if (strcmp(av[0], "cd") == 0) {
					if (end - start != 2) /* Si la commande n'a aucun argument retourne une erreur. */
						_err("error: cd: bad arguments\n", NULL);
					if (chdir(av[1])) /* Si la commande chdir ne fonctionne pas retourne une erreur. */
						_err("error: cd: cannot change directory to ", av[1]);
				}
				else if (execve(av[0], av, env)) /* Éxécution de la commande à l'aide des variables d'environnements. */
					_err("error: cannot execute ", av[0]);
				
				/* Libération de la mémoire utilisé et fermeture du processus avec un code de succès. */
				free(av);
				exit(0);
			} else { /* Processus parent */
				/* Attendre que le processus enfant est terminé puis fermeture de fd d'écriture. */
				waitpid(pid, NULL, 0);
				close(fd[1]);

				/* Fermeture du fd_in si la duplication n'a pas fonctionné. */
				if (fd_in)
					close(fd_in);

				/* . */
				fd_in = fd[0];

				/* Libération de la mémoire utilisé. */
				free(av);
			}
			/* On change la position à celle de la prochaine commande après le pipe. */
			start = end + 1;
		}
		/* Fermeture du fd_in. */
		close(fd_in);

		/* On change la position à celle de la prochaine commande après la semicolon. */
		i = pos_semicolon + 1;
	}
	return (0);
}