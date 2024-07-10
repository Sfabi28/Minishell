/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 09:21:04 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/06 12:27:35 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_envopwd(t_env **env, char *opwd)
{
	t_env	*temp;

	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, "OLDPWD", 7))
		{
			free(temp->value);
			temp->unsetted = 0;
			temp->value = ft_strdup(opwd);
			free(opwd);
			return (0);
		}
		temp = temp->next;
	}
	return (-1);
}

int	change_envpwd(t_env **env)
{
	t_env	*temp;
	char	*opwd;
	char	*cwd;

	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(temp->name, "PWD", 4))
		{
			opwd = ft_strdup(temp->value);
			free(temp->value);
			temp->unsetted = 0;
			cwd = getcwd(NULL, 0);
			temp->value = ft_strdup(cwd);
			free(cwd);
			return (change_envopwd(env, opwd));
		}
		temp = temp->next;
	}
	return (-1);
}

t_env	*env_lstnew(char *env, char **envp)
{
	t_env	*new;
	int		i;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	i = find_char(env, '=');
	if (i > 0)
	{
		new->name = ft_substr(env, 0, i);
		new->value = ft_strdup(&env[i + 1]);
	}
	else
	{
		new->name = ft_strdup(env);
		new->value = ft_strdup("");
	}
	new->envp = envp;
	new->unsetted = 0;
	new->next = NULL;
	return (new);
}

short	env_lstclear(t_env *lst)
{
	t_env	*temp;

	while (lst)
	{
		temp = lst->next;
		lst->envp = NULL;
		free(lst->name);
		free(lst->value);
		free(lst);
		lst = temp;
	}
	return (1);
}

t_env	*env_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
