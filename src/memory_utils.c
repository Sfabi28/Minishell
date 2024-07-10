/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 09:04:09 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/29 15:53:06 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_memory	*memory_lstnew(char *name, char *value)
{
	t_memory	*new;

	new = (t_memory *)malloc(sizeof(t_memory));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

void	memory_lstclear(t_memory *lst)
{
	t_memory	*temp;

	while (lst)
	{
		temp = lst->next;
		free(lst->name);
		free(lst->value);
		free(lst);
		lst = temp;
	}
}

void	env_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = env_lstlast(*lst);
	temp->next = new;
}

t_memory	*get_memory_node(t_memory *lst, char *name)
{
	int	len;
	int	len2;

	if (!lst)
		return (NULL);
	len = ft_strlen(name);
	while (lst)
	{
		len2 = ft_strlen(lst->name);
		if (len == len2 && !ft_strncmp(lst->name, name, len))
			return (lst);
		lst = lst->next;
	}
	return (lst);
}
