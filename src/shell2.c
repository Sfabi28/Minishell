/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:55:57 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/29 15:47:25 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_shell(t_env *env, t_line **line)
{
	t_env	*path;

	path = my_getenv(env, "PATH");
	if (path->unsetted)
	{
		(*line)->exit -= 2;
		write(2, "bash: env: No such file or directory\n", 37);
		return (handle_logic(line), 0);
	}
	while (env)
	{
		if (env->unsetted == 0 && env->value && ft_strlen(env->value) > 0)
			printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	handle_logic(line);
	return (0);
}

t_env	*my_getenv(t_env *env, char *name)
{
	int	len;
	int	len2;

	if (!name)
		return (NULL);
	len = ft_strlen(name);
	while (env)
	{
		len2 = ft_strlen(env->name);
		if (len == len2 && !ft_strncmp(env->name, name, len))
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	unset_shell(t_env **env, t_line **line)
{
	t_line	*tmp;
	t_env	*node;
	char	*name;

	tmp = *line;
	if (find_char(tmp->line, '=') > 0 || find_char(tmp->line, '/') > 0)
	{
		write(2, "bash: unset: ", 13);
		write(2, tmp->line + 6, len_to_first_space(tmp->line) - 6);
		write(2, ": not a valid identifier\n", 25);
		tmp->exit = -1;
		handle_logic(line);
		return (-1);
	}
	if (ft_strlen(tmp->line) < 7)
		return (handle_logic(line), 0);
	name = ft_substr(tmp->line, 6, len_to_first_space(tmp->line) - 6);
	node = my_getenv(*env, name);
	if (node)
		node->unsetted = 1;
	handle_logic(line);
	return (free(name), 0);
}

static	int	export_no_value(t_env **env)
{
	t_env	*temp;

	temp = *env;
	while (temp)
	{
		if (temp->unsetted == 0 && temp->value)
			printf("declare -x %s=\"%s\"\n", temp->name, temp->value);
		else if (temp->unsetted == 0)
			printf("declare -x %s\n", temp->name);
		temp = temp->next;
	}
	return (0);
}

int	export_shell(t_env **env, t_line **line)
{
	t_env	*last;
	char	*nm;
	t_env	*node;

	if (ft_strlen((*line)->line) < 8)
	{
		export_no_value(env);
		handle_logic(line);
		return (0);
	}
	nm = get_name(&((*line)->line[7]));
	node = my_getenv(*env, nm);
	if (!node)
	{
		last = env_lstlast(*env);
		last->next = env_lstnew(&((*line)->line[7]), last->envp);
	}
	else if (find_char((*line)->line, '=') > 0)
	{
		free(node->value);
		node->value = ft_strdup(&((*line)->line[7 + ft_strlen(nm) + 1]));
		node->unsetted = 0;
	}
	return (handle_logic(line), free(nm), 0);
}
