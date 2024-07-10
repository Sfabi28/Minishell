/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 10:59:59 by stescaro          #+#    #+#             */
/*   Updated: 2024/04/05 11:47:35 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_free(char *s)
{
	if (s)
		free(s);
	return (1);
}

void	file_error(char *file)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("No such file or directory", 2);
	ft_putstr_fd("\n", 2);
}

static	char	*back_path(char *path)
{
	int		i;
	char	*res;

	i = ft_strlen(path) - 1;
	while (i > 0 && path[i] == '/')
		i--;
	while (i > 0 && path[i] != '/')
		i--;
	while (i > 0 && path[i] == '/')
		i--;
	res = ft_substr(path, 0, i + 1);
	free(path);
	return (res);
}

static	char	*front_path(char *path, char *cmd, int len)
{
	char	*res;

	if (cmd[0] == '/')
		return (free(path), ft_strdup("/"));
	if (path[ft_strlen(path) - 1] != '/')
		path = f_strjoin(path, "/");
	res = join_and_free_sec(path, ft_substr(cmd, 0, len));
	free(path);
	return (res);
}

short	ret_of_chdir(char *cmd)
{
	char	*path;
	int		i;
	int		start;

	i = 0;
	path = ft_strjoin(getenv("PWD"), "/");
	while (cmd[i])
	{
		if (cmd[i] == '.' && cmd[i + 1] == '/')
			i += 2;
		else if (cmd[i] == '.' && cmd[i + 1] && cmd[i + 1] == '.'
			&& cmd[i + 2] == '/' && ++i && ++i && ++i)
			path = back_path(path);
		else
		{
			start = i;
			while (cmd[i] && cmd[i] != '/')
				i++;
			while (cmd[i] && cmd[i] == '/')
				i++;
			path = front_path(path, &cmd[start], i - start);
		}
	}
	i = access(path, F_OK);
	return (free(path), i);
}
