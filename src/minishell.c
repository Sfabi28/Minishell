/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 13:40:16 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:04 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
static	void	handle_par_logic(t_line **l, t_line **par)
{
	int		e;

	e = 0;
	if ((*par)->exit == 10)
	{
		(*l)->exit = 10;
		return ;
	}
	if ((*par)->exit == 0 && ++e)
		(*par)->exit = 1;
	else if (--e)
		(*par)->exit = -1;
	handle_logic(par);
	if (!(*par))
	{
		(*l)->exit = e;
		handle_logic(l);
	}
}*/

static	short	expr_valid_args(t_line **l)
{
	char	*line;
	int		i;

	line = (*l)->line;
	i = 5;
	while (line[i])
	{
		if (!is_space(line[i]) && !(line[i] >= '0' && line[i] <= '9')
			&& line[i] != '+' && line[i] != '-' && line[i] != '*'
			&& line[i] != '/' && line[i] != '%'
			&& (line[i] != '$' || line[i + 1] != '?')
			&& (line[i] != '?' || line[i - 1] != '$'))
		{
			printf("%s\n", &line[5]);
			free((*l)->line);
			(*l)->line = ft_strdup("expr ");
			break ;
		}
		i++;
	}
	return (1);
}

static	int	handle_par(t_data *data, t_line **par, int ret)
{
	t_line	*temp;
	int		res;
	int		pid;

	if (!par || !*par)
		return (-1);
	temp = *par;
	res = ret;
	pid = fork();
	if (pid == 0)
	{
		res = minishell(data, temp, res, -10);
		exit(res);
	}
	else if (pid > 0)
		waitpid(pid, &res, 0);
	(*par)->exit = !(WEXITSTATUS(res));
	return ((*par)->exit);
}

static	int	shell_cmds(t_data **data, t_line **tmp, t_env *path_node, int ret)
{
	int	unset;

	unset = path_node->unsetted;
	if ((*tmp)->type == 1)
		return (pipe_shell(*data, tmp));
	else if ((*tmp)->type == 2 && !unset)
		return (echo_shell(tmp, ret, &((*data)->echo), *data));
	else if ((*tmp)->type == 2 && (*tmp)->exit--)
		return (file_error("echo"), handle_logic(tmp), 0);
	else if ((*tmp)->type == 3)
		return (cd_shell(*data, tmp));
	else if ((*tmp)->type == 4)
		return (pwd_shell(tmp));
	else if ((*tmp)->type == 5)
		return (export_shell(&((*data)->env), tmp));
	else if ((*tmp)->type == 6)
		return (unset_shell(&((*data)->env), tmp));
	else if ((*tmp)->type == 7 && !unset)
		return (env_shell((*data)->env, tmp));
	else if ((*tmp)->type == 7 && (*tmp)->exit--)
		return (file_error("env"), handle_logic(tmp), 0);
	else if ((*tmp)->type == 8)
		return (memory_shell(*data, tmp));
	else
		return (exec_shell(*data, tmp));
}

int	minishell(t_data *data, t_line *tmp, int ret, int ext)
{
	t_env	*path_node;

	path_node = my_getenv(data->env, "PATH");
	while (tmp)
	{
		while (ret < 0)
			ret = 256 + ret;
		while (ret > 255)
			ret = ret - 256;
		if (tmp->type == -2)
			return (ret);
		else if (tmp->type == -1 || tmp->exit < -9)
			return (exit_shell(&tmp, ret, ext));
		else if (tmp->type == -3 && expr_valid_args(&tmp))
			return (expr_shell(&tmp, ret));
		else if (tmp->type < 9 && tmp->type > -1)
			ret = shell_cmds(&data, &tmp, path_node, ret);
		else if (tmp->type == 9)
		{
			ret = handle_par(data, &(tmp->parentesis), ret);
			tmp->exit = ret;
			handle_logic(&tmp);
		}
	}
	return (ret);
}
