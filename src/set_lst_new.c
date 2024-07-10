/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_lst_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 09:45:13 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/03 13:35:23 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_line	*new_empty_line(void)
{
	t_line	*new;

	new = malloc(sizeof(t_line));
	if (!new)
		return (NULL);
	new->type = -2;
	new->exit = 0;
	new->logic = 0;
	new->line = NULL;
	new->parentesis = NULL;
	new->next = NULL;
	return (new);
}

static	short	is_pipe(char *line)
{
	int	pip;
	int	maj;
	int	min;

	min = find_char(line, '<');
	maj = find_char(line, '>');
	pip = find_char(line, '|');
	if (pip < 0 && min < 0 && maj < 0)
		return (0);
	else if (!strncmp(line, "echo cat", 8))
		return (0);
	return (1);
}

short	set_logic(char *line)
{
	int	i;

	i = 0;
	while (line[i] && is_space(line[i]))
		i++;
	if (line[i] == '|' && line[i + 1] == '|')
		return (1);
	if (line[i] == '&' && line[i + 1] == '&')
		return (2);
	if (line[i] == ';')
		return (-1);
	return (0);
}

short	find_type(char *line)
{
	if (line[0] == '(')
		return (9);
	else if (ft_strncmp(line, "exit", 4) == 0 && is_space(line[4]))
		return (-1);
	else if (is_pipe(line))
		return (1);
	else if (ft_strncmp(line, "echo", 4) == 0 && is_space(line[4]))
		return (2);
	else if (ft_strncmp(line, "cd", 2) == 0 && is_space(line[2]))
		return (3);
	else if (ft_strncmp(line, "pwd", 3) == 0 && is_space(line[3]))
		return (4);
	else if (ft_strncmp(line, "export", 6) == 0 && is_space(line[6]))
		return (5);
	else if (ft_strncmp(line, "unset", 5) == 0 && is_space(line[5]))
		return (6);
	else if (ft_strncmp(line, "env", 3) == 0 && is_space(line[3]))
		return (7);
	else if (find_char(line, '=') > 0 && !is_space(line[find_char(line, '=')
				- 1]) && !is_space(line[find_char(line, '=') + 1]))
		return (8);
	else if (ft_strncmp(line, "expr", 4) == 0 && is_space(line[4]))
		return (-3);
	return (0);
}

void	line_lstclear(t_line *lst)
{
	t_line	*temp;

	while (lst)
	{
		temp = lst->next;
		if (lst->parentesis)
			line_lstclear(lst->parentesis);
		free(lst->line);
		free(lst);
		lst = temp;
	}
}
