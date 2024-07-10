/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 08:25:35 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 16:23:22 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	*delete_c(char *line, char c, int flag)
{
	char	*ret;
	int		i;
	int		j;

	if (flag > 0)
		return (ft_strdup(line));
	ret = ft_calloc(SL(line), 1);
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] != c)
			ret[j++] = line[i];
		i++;
	}
	return (ret);
}

int	odd_quotes(char *line, char c)
{
	int		i;
	int		quotes;

	i = 0;
	quotes = 0;
	while (line[i])
	{
		if (line[i] == c)
			quotes++;
		i++;
	}
	if (quotes % 2 == 0)
		return (0);
	return (1);
}

static	char	*handle_dquote(char c, int f)
{
	char	*prompt;
	char	*ret;
	char	*tmp;
	short	flag;

	prompt = ft_calloc(9 - (c == '\''), 1);
	if (!prompt)
		return (NULL);
	if (c == '\'')
		ft_strlcpy(prompt, "quote> ", 8);
	else
		ft_strlcpy(prompt, "dquote> ", 9);
	flag = 1;
	ret = NULL;
	while (flag)
	{
		tmp = readline(prompt);
		if (odd_quotes(tmp, c))
			flag = 0;
		tmp = join_and_free_sec("\n", tmp);
		ret = f_strjoin(ret, tmp);
		free(tmp);
	}
	tmp = delete_c(ret, c, f);
	return (free(prompt), free(ret), tmp);
}

void	handle_odd_quotes(char **tmp, int flag, int j, char c)
{
	char	*dq;
	int		k;

	dq = handle_dquote(c, flag);
	if (flag == -10)
	{
		ft_strlcat(*tmp, dq, 15000);
		free(dq);
		return ;
	}
	k = -1;
	while (dq[++k])
	{
		if (dq[k] != '$' && dq[k] != '(' && dq[k] != ')')
			(*tmp)[j++] = dq[k];
		else if (dq[k] == '$')
			(*tmp)[j++] = 36 + ((c == '\'') * 91);
		else
			(*tmp)[j++] = (dq[k] == ')') + 18;
	}
	free(dq);
}

void	handle_quote(char **t, int *i, char *l, t_echo **echo)
{
	int		flag;
	char	c;

	c = l[(*i) - 1];
	if (is_echo_quotes(*t))
		return (echo_quotes(t, l, i, echo));
	flag = is_limiter_or_filename(*i, l, c);
	while (!flag && l[*i] && l[*i] != c)
	{
		if (l[(*i)] != '$' && l[(*i)] != '(' && l[(*i)] != ')')
			(*t)[SL(*t)] = l[(*i)++];
		else if (l[(*i)] == '$' && ++(*i))
			(*t)[SL(*t)] = 36 + ((c == '\'') * 91);
		else if (l[(*i)] == '(' || l[(*i)] == ')')
			(*t)[SL(*t)] = (l[(*i++)] == ')') + 18 - 2 * (c == 39);
		else if (l[(*i)] == '*' && ++(*i))
			(*t)[SL(*t)] = 15;
	}
	if ((l[*i] == c && ++(*i)) && flag > 0)
		handle_limiter_or_filename(t, i - (l[*i - 1] == c), l, c);
	else if (flag == -1)
		handle_awk_marks(t, i - (l[*i - 1] == c), l, c);
	if (!(l[*i]) && odd_quotes(l, c) && flag >= 0)
		handle_odd_quotes(t, flag, SL(*t), c);
}
