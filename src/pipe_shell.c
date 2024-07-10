/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 13:45:48 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:08 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char	**parse_heredoc(char *line)
{
	char	**cmds;
	char	*tmp;
	char	*limiter;
	int		len;
	char	c;

	c = *line;
	if (c != '\"' && c != '\'')
		len = len_to_first_space(line);
	else
		len = find_char(&line[1], c) - 1;
	limiter = ft_substr(&line[(c == '\"' || c == '\'')], 0, len);
	if (!limiter)
		return (NULL);
	tmp = ft_substr(line, len + 1 + (c == '\"' || c == '\''),
			ft_strlen(&line[len + 1 - 2 * (len == (int)ft_strlen(line))
				+ (c == '\"' || c == '\'')]));
	if (!tmp)
		return (free(limiter), NULL);
	cmds = ft_split(tmp, '|');
	free(tmp);
	return (free(line), add_str_front_to_matrix(cmds, limiter));
}

char	**get_cmds(char *line, int start, int end, int fd_in)
{
	char	**cmds;
	char	*tmp;
	int		i;

	if (fd_in < 0 && fd_in != -10)
		return (NULL);
	tmp = remove_spaces(ft_substr(line, start, end - start + 1));
	if (!tmp)
		return (NULL);
	if (fd_in == -10)
		return (parse_heredoc(tmp));
	cmds = ft_split(tmp, '|');
	i = 0;
	while (cmds[i])
	{
		if (is_space(cmds[i][0]) || is_space(cmds[i][ft_strlen(cmds[i]) - 1]))
			cmds[i] = remove_spaces(cmds[i]);
		i++;
	}
	return (free(tmp), cmds);
}

static	int	open_outfile(char *line, int *end)
{
	int		fd;
	char	*file;
	char	c;
	int		flag;
	int		i;

	i = find_char(line, '>');
	if (i < 0)
	{
		*end = ft_strlen(line) - 1;
		return (-10);
	}
	*end = i - 1;
	flag = O_TRUNC;
	if (line[i + 1] == '>' && ++i)
		flag = O_APPEND;
	i++;
	i += skip_spaces(&line[i]);
	c = line[i];
	if (c != '\"' && c != '\'')
		file = ft_substr(&line[i], 0, len_to_first_space(&line[i]));
	else
		file = ft_substr(&line[i + 1], 0, find_char(&line[i + 1], c) - 1);
	fd = open(file, O_WRONLY | O_CREAT | flag, 0644);
	return (fd + (file && !ft_free(file)));
}

static	int	open_infile(char *line, int *start)
{
	int		fd;
	int		i;
	char	*file;
	char	c;
	int		len;

	i = find_char(line, '<');
	if (i < 0)
		return (dup(0));
	if (line[i + 1] == '<')
		*start = i + skip_spaces(&line[i + 2]) + 1;
	if (line[i + 1] == '<')
		return (-10);
	*start = i + skip_spaces(&line[i + 1]) + 1;
	c = line[*start];
	if (c != '\"' && c != '\'')
		len = len_to_first_space(&line[*start]);
	else
		len = find_char(&line[*start + 1], c) - 1;
	file = ft_substr(&line[*start], (c == '\"' || c == '\''), len);
	fd = open(file, O_RDONLY, 0644);
	if (fd < 0)
		return (file_error(file), -1);
	*start += len + skip_spaces(&line[*start + len]);
	return (free(file), fd);
}

int	pipe_shell(t_data *data, t_line **line)
{
	char	**cmds;
	int		start;
	int		end;

	data->fds = ft_calloc(sizeof(int), 4);
	if (!data->fds)
		return (-1);
	start = 0;
	end = 0;
	data->fds[0] = open_infile((*line)->line, &start);
	if (data->fds[0] < 0 && data->fds[0] != -10)
	{
		(*line)->exit = -1;
		return (free(data->fds), handle_logic(line), -1);
	}
	data->fds[1] = open_outfile((*line)->line, &end);
	if (data->fds[1] == -10)
		return (pipex_without_outfile(data, line, start));
	cmds = get_cmds((*line)->line, start, end, data->fds[0]);
	if (!cmds)
		return (free(data->fds), handle_logic(line), -1);
	end = pipex(data, cmds);
	if (end < 0)
		(*line)->exit = -1;
	return (free(data->fds), ft_freesplit(cmds), handle_logic(line), end);
}
