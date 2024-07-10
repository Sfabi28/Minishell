/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pipe_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 10:59:52 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:23 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	do_heredoc(char *s)
{
	char	*limiter;
	char	*line;
	int		pid;

	pid = skip_spaces(s);
	if (s[pid] == '\"' || s[pid] == '\'')
		limiter = ft_substr(&s[pid], 1, find_char(&s[pid + 1], s[pid]));
	else
		limiter = ft_substr(&s[pid], 0, find_char(&s[pid], ' '));
	line = NULL;
	pid = fork();
	if (pid == 0 && s_s(2, NULL))
	{
		while (write(1, "pipe heredoc> ", 14) && read_gnl(&line))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
				&& line[ft_strlen(limiter)] == '\n')
				exit((ft_free(line) + s_s(0, NULL)) * 0);
			free(line);
		}
		exit(h_s(0, NULL) * 0);
	}
	else if (pid > 0 && s_s(1, NULL))
		waitpid(pid, NULL, 0);
	return (ft_free(limiter) - 1);
}

static	int	handle_outfiles(char *s)
{
	int		i;
	char	*fn;
	int		fd;
	int		flag;

	i = find_char(s, '>');
	if (i < 0)
		return (ft_strlen(s));
	else if (s[i + 1] && find_char(&s[i + 2], '>') < 0)
		return (i);
	while (s[i] && s[i + 1] && find_char(&s[i + 2], '>') > -1)
	{
		flag = O_TRUNC;
		if (++i && s[i] == '>' && ++i)
			flag = O_APPEND;
		while (s[i] && is_space(s[i]))
			i++;
		fn = ft_substr(&s[i], 0, find_char(&s[i], ' '));
		fd = open(fn, O_WRONLY | O_CREAT | flag, 0777);
		(void)(close(fd) + ft_free(fn));
		i += find_char(&s[i], '>');
	}
	return (i - 1);
}

static	int	handle_infiles(char *s)
{
	int		i;
	char	*fn;
	int		fd;

	i = find_char(s, '<');
	if (i < 0 || (s[i + 1] && find_char(&s[i + 2], '<') < 0))
		return (0);
	while (s[i] && s[i + 1] && find_char(&s[i + 2], '<') > -1)
	{
		if (s[i] == '<' && s[i + 1] == '<')
			i += 2 + 0 * do_heredoc(&s[i + 2]);
		else if (s[i++] == '<')
		{
			while (s[i] && is_space(s[i]))
				i++;
			fn = ft_substr(&s[i], 0, find_char(&s[i], ' '));
			fd = open(fn, O_RDONLY, 0777);
			if (fd < 0)
				return (file_error(fn), free(fn), ft_strlen(s));
			(void)(close(fd) + ft_free(fn));
		}
		i += find_char(&s[i], '<');
	}
	return (i);
}

static	char	*handle_multiple_files(char *s)
{
	int		start;
	int		of_start;
	char	*tmp;
	char	*ret;
	int		len;

	start = handle_infiles(s);
	len = ft_strlen(s);
	if (start == len)
	{
		ret = ft_strdup("echo -n");
		return (ret);
	}
	if (find_char(s, '>') < start)
		return (ft_substr(s, start, len));
	tmp = ft_substr(s, start, find_char(&s[start], '>'));
	of_start = start + handle_outfiles(&s[start]);
	ret = join_and_free_sec(tmp, ft_strdup(&s[of_start]));
	free(tmp);
	return (ret);
}

char	*set_pipe_line(char *s)
{
	int		i;
	char	*ret;
	char	*before;
	char	*after;

	ret = handle_multiple_files(s);
	i = find_char(ret, '<') + ft_free(s) - 1;
	if (i > 0)
	{
		before = ft_substr(ret, 0, i - is_space(ret[i - 1]));
		if (find_char(&ret[i], '|') > -1)
		{
			after = ft_substr(&ret[i], 0, find_char(&ret[i], '|'));
			s = ft_strdup(&ret[i + find_char(&ret[i], '|')]);
		}
		else
		{
			after = ft_strdup(&ret[i]);
			s = ft_strdup("");
		}
		free(ret);
		ret = f_strjoin(f_strjoin(ft_strjoin(after, " "), before), s);
		(void)(ft_free(before) + ft_free(after) + ft_free(s));
	}
	return (ret);
}
