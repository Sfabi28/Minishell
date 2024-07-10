/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_strjoin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 09:41:07 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/07 12:50:14 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_and_free_sec(char *s1, char *s2)
{
	char	*str;

	str = ft_strjoin(s1, s2);
	free(s2);
	return (str);
}

char	*f_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	l1;
	size_t	l2;
	size_t	i;

	if (!s1)
		return (ft_strdup(s2));
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	str = (char *) ft_calloc((l1 + l2 + 1), sizeof(char));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < l1)
	{
		str[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < l2)
	{
		str[i + l1] = s2[i];
		i++;
	}
	return (free(s1), str);
}

t_echo	*echo_lstadd_back(t_echo **lst, t_echo *new)
{
	t_echo	*temp;

	if (!*lst)
	{
		*lst = new;
		return (new);
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (new);
}

t_echo	*echo_lstnew(char *str, char c)
{
	t_echo	*new;

	new = (t_echo *)malloc(sizeof(t_echo));
	if (!new)
		return (NULL);
	if (c == '\"')
		new->quotes = 1;
	else
		new->quotes = 0;
	new->str = ft_strdup(str);
	new->printed = 0;
	new->next = NULL;
	return (new);
}

short	echo_lst_clear(t_echo *lst)
{
	t_echo	*temp;

	while (lst)
	{
		temp = lst->next;
		free(lst->str);
		free(lst);
		lst = temp;
	}
	return (1);
}

/*int main()
{
	printf("%s\n", ft_strjoin("ok,", " funziona"));
	return 0;
}*/
