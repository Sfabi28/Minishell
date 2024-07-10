/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:28:16 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:13 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	child(t_data *data, char *argv, t_env *env, int ret)
{
	int	pid;
	int	arr[2];

	if (pipe(arr) < 0)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(arr[0]);
		dup2(arr[1], 1);
		close(arr[1]);
		close_fds(data->fds);
		exit(ft_execve(argv, env, 0));
	}
	else if (pid > 0)
	{
		close(arr[1]);
		dup2(arr[0], 0);
		waitpid(pid, &ret, 0);
	}
	return (WEXITSTATUS(ret) - (!(close(arr[0]) + close(arr[1])) && pid < 1));
}

static	int	children_exe(int argc, char **argv, t_data *data, int ret)
{
	int	i;

	i = -1;
	while (++i < argc)
	{
		ret += child(data, argv[i], data->env, 0);
		if (ret < 0 || ret > 26)
			return (-1);
	}
	return (0);
}

static void	ft_heredoc(char **argv, t_data *data, int *res)
{
	char	*line;
	int		arr[2];
	int		pid;

	line = NULL;
	if (pipe(arr) < 0)
		return ;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0 && s_s(2, NULL) && !close(arr[0]))
	{
		close_fds(data->fds);
		while (write(1, "pipe heredoc> ", 14) && read_gnl(&line))
		{
			if (ft_strncmp(line, argv[0], ft_strlen(argv[0])) == 0
				&& line[ft_strlen(argv[0])] == '\n')
				exit(close(arr[1]) + ft_free(line) + s_s(0, res));
			(void)(write(arr[1], line, ft_strlen(line)) + ft_free(line));
		}
		exit(close(arr[1]) + h_s(0, res));
	}
	else if (pid > 0 && s_s(1, NULL)
		&& dup2(arr[0], close(arr[1])) > -1 && !close(arr[0]))
		wait(NULL);
}

static	void	check_cmds(int *ret, int argc, char **argv, t_env *env)
{
	char	*cmd;
	char	*path;
	int		i;
	short	useless;

	if (*ret == 0 || *ret == 130 || *ret == 131)
		return ;
	*ret = 127;
	i = 0;
	useless = 0;
	while (i < argc + 1)
	{
		cmd = ft_substr(argv[i], 0, find_char(argv[i], ' '));
		path = get_path(cmd, env);
		if (!path)
			cmd_not_found(cmd, &useless);
		else if (path[0] == '\0')
			file_error(cmd);
		if (path)
			free(path);
		free(cmd);
		i++;
	}
}

int	pipex(t_data *data, char **argv)
{
	int	argc;
	int	ret;
	int	f;

	f = 0;
	ret = 0;
	argc = 0;
	data->fds[2] = dup(0);
	data->fds[3] = dup(1);
	while (argv[argc + 1])
		argc++;
	if (++f && data->fds[0] == -10)
		ft_heredoc(argv, data, &ret);
	else if (data->fds[0] && f--)
	{
		dup2(data->fds[0], 0);
		close(data->fds[0]);
	}
	ret = children_exe(argc - f, &argv[f], data, ret);
	(void)(data->fds[1] == 1 || (dup2(data->fds[1], 1) + close(data->fds[1])));
	ret += ft_execve(argv[argc], data->env, 0);
	dup2(data->fds[3], 1);
	dup2(data->fds[2], 0);
	check_cmds(&ret, argc - f, &argv[f], data->env);
	return ((close(data->fds[2]) + close(data->fds[3])) * 0 + ret);
}
