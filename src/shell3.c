/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 08:52:00 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:32 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_echo_quotes(t_echo **echo, t_data *data, int ret)
{
	t_echo	*tmp;
	int		i;

	tmp = *echo;
	while (tmp && tmp->printed)
		tmp = tmp->next;
	tmp->printed = 1;
	i = 0;
	while (tmp->str[i])
	{
		if (tmp->quotes && tmp->str[i] == '$' && tmp->str[i + 1] == '?')
			i += printf("%d", ret) * 0 + 2;
		else if (tmp->quotes && tmp->str[i] == '$' && tmp->str[i + 1]
			&& ((tmp->str[i + 1] > 47 && tmp->str[i + 1] < 58)
				|| (tmp->str[i + 1] > 64 && tmp->str[i + 1] < 91)
				|| (tmp->str[i + 1] > 96 && tmp->str[i + 1] < 123)))
			i += print_quote_ref(&(tmp->str[i]), data);
		else
			i += printf("%c", tmp->str[i]);
	}
}

static	int	find_n(char *line, int *i, int ret)
{
	int	n;

	while (line[*i] && is_space(line[*i]))
		(*i)++;
	if (line[*i] == '$' && line[*i + 1] == '?')
	{
		(*i) += 2;
		return (ret);
	}
	n = ft_atoi(&line[*i]);
	if (line[*i] == '-')
		(*i)++;
	while (line[*i] >= '0' && line[*i] <= '9')
		(*i)++;
	while (line[*i] && is_space(line[*i]))
		(*i)++;
	return (n);
}

static	char	find_operator(char *line, int *i)
{
	char	operator;

	while (line[*i] && line[*i] != '+' && line[*i] != '-' && line[*i] != '*'
		&& line[*i] != '/' && line[*i] != '%')
		(*i)++;
	operator = line[*i];
	if (operator)
		(*i)++;
	while (line[*i] && is_space(line[*i]))
		(*i)++;
	return (operator);
}

int	expr_shell(t_line **line, int ret)
{
	int			i;
	long long	res;
	int			n;
	char		operator;

	i = 5;
	res = find_n((*line)->line, &i, ret);
	while ((*line)->line[i])
	{
		operator = find_operator((*line)->line, &i);
		n = find_n((*line)->line, &i, ret);
		if (operator == '+')
			res += n;
		else if (operator == '-')
			res -= n;
		else if (operator == '*')
			res *= n;
		else if (operator == '/' && n != 0)
			res /= n;
		else if (operator == '/' && --(*line)->exit < 2 && printf("expr:"))
			return (handle_logic(line), printf(" division by zero\n") * 0 + 1);
		else if (operator == '%')
			res %= n;
	}
	return (handle_logic(line), (i > 5 && printf("%lld\n", res)) * 0);
}

int	exit_shell(t_line **line, int ret, int ext)
{
	char	*l;
	int		i;
	char	*tmp;
	int		n;

	l = (*line)->line;
	i = 4;
	printf("exit\n");
	while (l[i] && is_space(l[i]))
		i++;
	if (!l[i])
		return (handle_logic(line), ext);
	tmp = ft_substr(&l[i], 0, len_to_first_space(&l[i]));
	n = find_n(l, &i, ret);
	while (n < 0)
		n = 256 + n;
	while (n > 255)
		n = n - 256;
	while (l[i] && is_space(l[i]))
		i++;
	if (l[i] && is_space(l[i]) && printf("exit: too many arguments\n"))
		return (handle_logic(line), free(tmp), 1);
	else if (l[i] && printf("exit: %s: numeric argument required\n", tmp))
		return (handle_logic(line), free(tmp), -12);
	return (free(tmp), -(10 + n));
}
