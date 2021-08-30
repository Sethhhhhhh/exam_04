#include "microshell.h"

static size_t	s_strlen(char *str) {
	size_t	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i])
		i++;
	return (i);
}

static char	*s_strdup(char *str) {
	char	*new;
	size_t 	i;

	new = (char *) malloc(sizeof(char) * (s_strlen(str) + 1));
	if (!new)
		return (NULL);
	for (i = 0; str[i]; i++) {
		new[i] = str[i];
	}
	new[i] = 0;
	return (new);
}

static int	add_arg(t_stack *stack, char *arg) {
	char			**tmp;
	unsigned int	i;

	tmp = (char **) malloc(sizeof(char *) * (stack->size + 2));
	if (!tmp)
		return (1);
	for (i = 0; i < stack->size; i++)
		tmp[i] = stack->args[i];
	if (stack->size > 0)
		free(stack->args);
	tmp[i++] = s_strdup(arg);
	tmp[i] = 0;
	stack->args = tmp;
	stack->size += 1;
	return (0);
}

static char	push(t_stack **stack, char *arg) {
	t_stack	*new;

	new = (t_stack *) malloc(sizeof(t_stack));
	if (!new)
		return (1);
	new->args = NULL;
	new->type = TYPE_END;
	new->previous = NULL;
	new->next = NULL;
	if (*stack) {
		(*stack)->next = new;
		new->previous = *stack;
	}
	*stack = new;
	return (add_arg(new, arg));
}

static char	parse(t_stack **stack, char *arg) {
	char    sep;

	sep = !strncmp(";", arg, 1);
	if (sep && !(*stack))
		return (0);
	else if (!sep && (!*stack || (*stack)->type != TYPE_END))
		push(stack, arg);
	else if (!strncmp("|", arg, 1))
		(*stack)->type = TYPE_PIPE;
	else if (sep)
		(*stack)->type = TYPE_BREAK;
	else
		add_arg(*stack, arg);

	return (0);
}

int	main(int ac, char **av, char **env) {
	t_stack  *stack;

	stack = NULL;
	for (unsigned int i = 1; i < ac; i++) {
		parse(&stack, av[i]);
	}
	while (stack && stack->previous)
		stack = stack->previous;
	while (stack) {

		printf("cmd:  ");
		for (size_t i = 0; stack->args[i]; i++) {
			printf("%s ", stack->args[i]);
		}
		printf(" | type: ");
		if (stack->type == TYPE_BREAK)
			printf("TYPE_BREAK\n");
		else if (stack->type == TYPE_END)
			printf("TYPE_END\n");
		else if (stack->type == TYPE_PIPE)
			printf("TYPE_PIPE\n");
		stack = stack->next;
	}

	return (0);
}