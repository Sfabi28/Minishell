/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:28:25 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:16 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_gnl(char **line)
{
	char	*buffer;
	int		i;
	int		r;
	char	c;

	i = 0;
	r = 0;
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	r = read(0, &c, 1);
	while (r && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		r = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = ft_strdup(buffer);
	if (!(*line))
		return (free(buffer), -1);
	free(buffer);
	return (r);
}

char	**add_str_front_to_matrix(char **mtx, char *s)
{
	char	**new_mtx;
	int		i;

	i = 0;
	while (mtx[i])
		i++;
	new_mtx = malloc(sizeof(char *) * (i + 2));
	if (!new_mtx)
		return (NULL);
	new_mtx[0] = ft_strdup(s);
	if (!new_mtx[0])
		return (free(new_mtx), ft_freesplit(mtx));
	i = -1;
	while (mtx[++i])
	{
		new_mtx[i + 1] = ft_strdup(mtx[i]);
		if (!new_mtx[i + 1])
		{
			ft_freesplit(mtx);
			return (ft_freesplit(new_mtx));
		}
	}
	ft_freesplit(mtx);
	new_mtx[i + 1] = NULL;
	return (free(s), new_mtx);
}

void	close_fds(int *fds)
{
	if (fds[0] > 0)
		close(fds[0]);
	if (fds[1] > 1)
		close(fds[1]);
	close(fds[2]);
	close(fds[3]);
	close(37);
	close(38);
	close(39);
	close(103);
}

int	pipex_without_outfile(t_data *data, t_line **l, int i)
{
	char	**cmds;
	int		count_cat;
	int		res;
	char	*tmp;
	char	*line;

	data->fds[1] = 127;
	line = (*l)->line;
	cmds = get_cmds(line, i, ft_strlen(line) - 1, data->fds[0]);
	count_cat = 0;
	while (cmds[count_cat] && !strncmp(cmds[count_cat], "cat", 3)
		&& (!cmds[count_cat][3] || (is_space(cmds[count_cat][3])
		&& !cmds[count_cat][4])))
		count_cat++;
	res = pipex(data, &cmds[count_cat - (!cmds[count_cat])]);
	close_fds(data->fds);
	if (res < 0)
		(*l)->exit = -1;
	if (!count_cat || !cmds[count_cat])
		return (free(data->fds), ft_freesplit(cmds), handle_logic(l), res);
	while (count_cat--)
		(void)(read_gnl(&tmp) + ft_free(tmp));
	return (free(data->fds), ft_freesplit(cmds), handle_logic(l), res);
}
