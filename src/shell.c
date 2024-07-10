/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:22:32 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:27 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_logic(t_line **line)
{
	int		logic;

	logic = (*line)->logic;
	if (logic == 1 && (*line)->exit > 0)
	{
		while ((*line) && (*line)->logic == 1)
			(*line) = (*line)->next;
	}
	else if (logic == 1 && (*line)->exit < 1 && (*line)->next
		&& (*line)->next->logic != 1)
		(*line)->next->logic = 3;
	else if ((logic == 2 || logic == 3) && (*line)->exit < 1)
	{
		while ((*line) && (*line)->next)
			(*line) = (*line)->next;
	}
	(*line) = (*line)->next;
}

int	echo_shell(t_line **l, int ret, t_echo **echo, t_data *data)
{
	int		i;
	char	*t;
	int		f;

	i = 4;
	t = (*l)->line;
	f = 0;
	if (!t[i])
		return (handle_logic(l), printf("\n") - 1);
	if (t[i++] && t[i] == '-' && t[i + 1] == 'n' && is_space(t[i + 2]) && ++f)
		i += 3;
	if (!t[i - 1])
		return (handle_logic(l), 0);
	while (t[i])
	{
		if (t[i] == 14 && ++i)
			print_echo_quotes(echo, data, ret);
		else if (t[i] == '$' && t[i + 1] == '?')
			i += 0 * printf("%d", ret) + 2;
		else
			i += printf("%c", t[i] + 24 * (t[i] == 16 || t[i] == 17)
					+ 22 * (t[i] == 18 || t[i] == 19));
	}
	(void)((f) && printf("\e[38;2;0;0;0m\e[48;2;255;255;255m%%\033[0m"));
	return (handle_logic(l), printf("\n") - 1);
}

int	memory_shell(t_data *data, t_line **line)
{
	char		*l;
	char		*name;
	char		*value;
	int			i;
	t_memory	*node;

	l = (*line)->line;
	i = find_char(l, '=');
	if (i < 1 || is_space(l[i - 1]) || is_space(l[i + 1]) || !l[i + 1])
	{
		(*line)->exit -= 2;
		return (handle_logic(line), -1);
	}
	name = ft_substr(l, 0, i);
	value = ft_substr(l, i + 1, ft_strlen(l) - i - 1);
	node = get_memory_node(data->memory, name);
	if (!node)
		memory_lstadd_back(&(data->memory), memory_lstnew(name, value));
	else
	{
		free(node->value);
		node->value = ft_strdup(value);
	}
	handle_logic(line);
	return (free(name), free(value), 0);
}

int	cd_shell(t_data *data, t_line **line)
{
	char	*tmp;
	char	*cmd;
	int		i;

	tmp = (*line)->line;
	i = 2;
	if (!tmp[i] || tmp[i + 1] == '~')
	{
		chdir(getenv("HOME"));
		if ((!tmp[i] || !tmp[i + 2]) && (change_envpwd(&(data->env)) || i > 0))
			return (handle_logic(line), 0);
		i += 2;
	}
	cmd = ft_substr(tmp, i + 1, len_to_first_space(&tmp[i + 1]));
	if (!cmd)
	{
		(*line)->exit -= 2;
		return (handle_logic(line), -1);
	}
	if (chdir(cmd) && write(2, "cd: no such file or directory: ", 32))
	{
		(*line)->exit -= (write(2, cmd, ft_strlen(cmd)) + write(2, "\n", 1));
		return (free(cmd), handle_logic(line), -1);
	}
	return (handle_logic(line), free(cmd), change_envpwd(&data->env));
}

int	pwd_shell(t_line **line)
{
	char	*tmp;

	tmp = getcwd(NULL, 0);
	if (!tmp)
	{
		(*line)->exit -= 2;
		return (handle_logic(line), -1);
	}
	printf("%s\n", tmp);
	handle_logic(line);
	free(tmp);
	return (0);
}
