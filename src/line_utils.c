/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 09:30:06 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:20:58 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	len_ref(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '\'' && !is_space(s[i])
		&& s[i] != '$' && s[i] != '\"')
		i++;
	return (i);
}

static	int	handle_ref(t_memory *memory, t_env *env, char *tmp, char **ret)
{
	int			j;
	char		*ref;

	j = len_ref(tmp);
	ref = ft_substr(tmp, 0, j);
	if (!ref)
		return (j);
	while (memory)
	{
		if (!ft_strncmp(memory->name, ref, ft_strlen(ref)))
			ft_strlcat(*ret, memory->value, 25000);
		if (!ft_strncmp(memory->name, ref, ft_strlen(ref)))
			return (free(ref), j);
		memory = memory->next;
	}
	while (env)
	{
		if (!ft_strncmp(env->name, ref, ft_strlen(ref)))
		{
			ft_strlcat(*ret, env->value, 25000);
			return (free(ref), j - (tmp[j] == '\''));
		}
		env = env->next;
	}
	return (free(ref), j);
}

static	int	ref_parenteses(char *tmp, char **ret, short flag)
{
	int		i;

	if (!flag)
	{
		(*ret)[ft_strlen(*ret)] = tmp[0];
		return (0);
	}
	i = 1;
	(*ret)[ft_strlen(*ret)] = 40;
	while (tmp[i] && (tmp[i] != ')' || tmp[i] != 19))
	{
		if (tmp[i] == '(' || tmp[i] == 18)
			i += ref_parenteses(&tmp[i], ret, (i > 0 && tmp[i - 1] == '$'));
		else
			(*ret)[ft_strlen(*ret)] = tmp[i];
		i++;
	}
	return (i + (tmp[0] != '$') * find_char(&tmp[i], ')'));
}

char	*check_ref(char *tmp, t_memory *memory, t_env *env)
{
	int		i;
	char	*ret;

	i = -1;
	ret = ft_calloc(25000, 1);
	while (tmp[++i])
	{
		if (tmp[i] == '(' || tmp[i] == 18)
			i += ref_parenteses(&tmp[i], &ret, (i > 0 && tmp[i - 1] == '$'));
		else if ((tmp[i] != 127 && tmp[i] != 36 && tmp[i] != 42 && tmp[i] != 15)
			|| (tmp[i] == '$' && (tmp[i + 1] == '?' || tmp[i + 1] == 18
					|| tmp[i + 1] == '(' || tmp[i + 1] == 59 || tmp[i + 1] == 38
					|| tmp[i + 1] == '|' || tmp[i + 1] == 127)))
			ret[ft_strlen(ret)] = tmp[i];
		else if (tmp[i] == 15)
			ret[ft_strlen(ret)] = '*';
		else if (tmp[i] == '*')
			i = handle_wcards(tmp, i, &ret);
		else if (tmp[i] == 127 || is_space(tmp[i + 1])
			|| tmp[i + 1] == 127 || tmp[i + 1] == '$')
			ret[ft_strlen(ret)] = 36;
		else if (++i)
			i += handle_ref(memory, env, &tmp[i], &ret) - 1;
	}
	return (ret);
}

short	nq(char *s, int times)
{
	int		i;
	char	*left;

	i = 0;
	while (times)
	{
		while (s[i] && strncmp(&s[i], "/bin/", 5))
			i++;
		if (s[i])
		{
			i += 5;
			times--;
		}
	}
	left = ft_substr(s, 0, i - 5);
	if ((odd_quotes(left, '\'') && odd_quotes(&s[i], '\''))
		|| (odd_quotes(left, '\"') && odd_quotes(&s[i], '\"')))
		return (free(left), 0);
	return (free(left), 1);
}
