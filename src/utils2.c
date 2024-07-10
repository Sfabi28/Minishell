/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 12:06:33 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 08:44:56 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	memory_lstadd_back(t_memory **lst, t_memory *new)
{
	t_memory	*temp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_line	*set_parentesis(char *line)
{
	char	*tmp;
	t_line	*new;

	if (!line)
		return (NULL);
	if (line[0] != '(')
		return (NULL);
	tmp = ft_substr(line, 1, ft_strlen(line) - 2);
	new = ft_line_lstnew(tmp);
	free(tmp);
	return (new);
}

char	*remove_spaces(char *s)
{
	int		start;
	int		end;
	char	*res;

	if (!s)
		return (ft_strdup(""));
	start = skip_spaces(s);
	end = ft_strlen(s) - 1;
	while (end > 0 && is_space(s[end]))
		end--;
	res = ft_substr(s, start, end - start + 1);
	free(s);
	return (res);
}

void	cmd_not_found(char *cmd, short *res)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("\n", 2);
	*res = 128 + *res;
}

const char	*prompt_location(t_env *env)
{
	t_env	*temp;
	char	*prompt;
	char	*location;

	temp = env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, "PWD", 4))
			break ;
		temp = temp->next;
	}
	if (!temp)
		return (NULL);
	location = ft_strjoin(temp->value, "\033[0m ");
	prompt = ft_strjoin("\033[1;92mMinishell$~\033[1;34m", location);
	free(location);
	return (prompt);
}
