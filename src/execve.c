/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 08:57:21 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:20:51 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *str, t_env *env)
{
	t_env	*node;
	char	**paths;
	char	*temp;
	char	*tmp;
	int		i;

	node = my_getenv(env, "PATH");
	if (!node)
		return (NULL);
	if (node->unsetted == 1)
		return (file_error(str), ft_strdup(""));
	paths = ft_split(node->value, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		tmp = ft_strjoin(temp, str);
		free(temp);
		if (access(tmp, F_OK) == 0)
			return (ft_freesplit(paths), tmp);
		free(tmp);
	}
	return (ft_freesplit(paths), NULL);
}

static	char	**clr_awk(char *str)
{
	int		i;
	int		j;
	int		flag;
	int		file;
	char	**clr;

	if (!str)
		return (NULL);
	flag = n_flags_awk(str);
	file = is_file(str);
	clr = ft_calloc(sizeof(char *), flag + (file > 0) + 3);
	clr[0] = ft_strdup("awk");
	i = -1;
	j = 0;
	while (++i < flag)
		clr[i + 1] = set_flag(str, &j);
	if (file > 0)
	{
		clr[i + 1] = ft_substr(str, j + 1, file - j - 4);
		clr[i + 2] = get_file(str, file);
	}
	else
		clr[i + 1] = ft_substr(str, j + 1, ft_strlen(str) - j - 2);
	return (clr);
}

short	ft_execve(char *str, t_env *env, int res)
{
	int		pid;
	char	**clr;
	char	*path;

	if (!strncmp(str, "bash", 4) && str[4] == '@')
		clr = ft_split(str, '@');
	else if ((!strncmp(str, "awk", 3) && is_space(str[3])))
		clr = clr_awk(&str[4 + 2 * (str[0] == '\'' || str[0] == '\"')
				- (!str[3 + 2 * (str[0] == '\'' || str[0] == '\"')])]);
	else
		clr = ft_split(str, ' ');
	if (!clr)
		return (s_s(0, &res) * 0);
	path = get_path(clr[0], env);
	if (!path || !path[0])
		return (ft_freesplit(clr), s_s(0, &res) * 0 - !(path && ft_free(path)));
	pid = fork();
	if (pid == 0 && s_s(2, NULL) && execve(path, clr, env->envp) < 0 && --res)
		return (ft_freesplit(clr), free(path), s_s(0, &res) * 0 + res);
	else if (pid > 0 && s_s(1, NULL))
		waitpid(pid, &res, 0);
	if (ft_free(path) && !ft_strncmp(str, "clear", 5) && is_space(str[5]))
		header();
	res = WEXITSTATUS(res);
	return (ft_freesplit(clr), s_s(0, &res) * 0 + res - (pid < 0));
}

int	exec_shell(t_data *data, t_line **line)
{
	t_line	*temp;
	char	*l;
	short	res;

	temp = *line;
	l = temp->line;
	res = ft_execve(l, data->env, 0);
	if (res < 0)
	{
		cmd_not_found(l, &res);
		temp->exit = -1;
	}
	handle_logic(line);
	return ((int)res);
}
