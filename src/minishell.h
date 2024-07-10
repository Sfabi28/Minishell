/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfabi <sfabi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:23:38 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 10:58:35 by sfabi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SL ft_strlen

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <sys/mman.h>
# include "libft.h"

typedef struct s_env
{
	char			**envp;
	char			*name;
	char			*value;
	short			unsetted;
	struct s_env	*next;
}	t_env;

typedef struct s_memory
{
	char			*name;
	char			*value;
	struct s_memory	*next;
}	t_memory;

typedef struct s_line
{
	struct s_line	*parentesis;
	short			type;
	char			*line;
	short			logic;
	short			exit;
	struct s_line	*next;
}	t_line;

typedef struct s_echo
{
	char			*str;
	short			printed;
	short			quotes;
	struct s_echo	*next;
}	t_echo;

typedef struct s_data
{
	struct s_env	*env;
	struct s_line	*line;
	struct s_memory	*memory;
	struct s_echo	*echo;
	int				*fds;
}	t_data;

extern int	g_ctrl;

t_env		*my_getenv(t_env *env, char *name);
short		ft_execve(char *str, t_env *env, int res);
short		is_space(char c);
t_env		*env_lstlast(t_env *lst);
t_env		*env_lstnew(char *env, char **envp);
short		env_lstclear(t_env *lst);
void		env_lstadd_back(t_env **lst, t_env *new);
int			len_to_first_space(char *s);
int			find_char(char *s, char c);
int			header(void);
char		*remove_marks(char *line, t_memory *memory, t_env *env, t_echo **e);
char		*f_strjoin(char *s1, char *s2);
void		handle_quote(char **t, int *i, char *line, t_echo **echo);
char		*check_ref(char *tmp, t_memory *memory, t_env *env);
t_line		*line_lstnew(char *line, t_memory *memory, t_env *env, t_echo **e);
char		*join_and_free_sec(char *s1, char *s2);
short		find_type(char *line);
short		set_logic(char *line);
int			skip_spaces(char *s);
void		line_lstclear(t_line *lst);
void		memory_lstclear(t_memory *lst);
t_memory	*memory_lstnew(char *name, char *value);
void		memory_lstadd_back(t_memory **lst, t_memory *new);
void		handle_logic(t_line **line);
int			mem_shell(t_data *data, t_line **line);
short		ret_of_chdir(char *cmd);
t_line		*ft_line_lstnew(char *line);
int			read_gnl(char **line);
char		**add_str_front_to_matrix(char **mtx, char *s);
int			pipex(t_data *data, char **argv);
t_env		*_getenv(t_env *env, char *name);
short		is_limiter_or_filename(int i, char *l, char c);
void		handle_limiter_or_filename(char **tmp, int *i, char *line, char c);
int			minishell(t_data *data, t_line *tmp, int ret, int ext);
int			pipe_shell(t_data *data, t_line **line);
int			cd_shell(t_data *data, t_line **line);
int			pwd_shell(t_line **line);
int			memory_shell(t_data *data, t_line **line);
int			export_shell(t_env **env, t_line **line);
int			unset_shell(t_env **env, t_line **line);
int			env_shell(t_env *env, t_line **line);
int			echo_shell(t_line **l, int ret, t_echo **echo, t_data *data);
void		handle_logic(t_line **line);
int			exec_shell(t_data *data, t_line **line);
t_line		*set_parentesis(char *line);
int			change_envpwd(t_env **env);
int			change_envopwd(t_env **env, char *opwd);
char		*remove_spaces(char *s);
void		cmd_not_found(char *cmd, short *res);
void		file_error(char *file);
int			ft_free(char *s);
char		*set_flag(char *s, int *j);
int			is_file(char *str);
int			n_flags_awk(char *str);
void		handle_awk_marks(char **tmp, int *i, char *line, char c);
char		*get_file(char *str, int i);
t_line		*new_empty_line(void);
char		*get_path(char *str, t_env *env);
const char	*prompt_location(t_env *env);
char		*get_name(char *s);
t_memory	*get_memory_node(t_memory *lst, char *name);
char		**get_cmds(char *line, int start, int end, int fd_in);
int			pipex_without_outfile(t_data *data, t_line **l, int i);
int			odd_quotes(char *line, char c);
short		nq(char *s, int times);
void		close_fds(int *fds);
int			handle_wcards(char *line, int i, char **res);
int			expr_shell(t_line **line, int ret);
int			exit_shell(t_line **line, int ret, int ext);
void		signal_handler(int signum);
void		echo_quotes(char **tmp, char *l, int *i, t_echo **echo);
void		handle_odd_quotes(char **tmp, int flag, int j, char c);
t_echo		*echo_lstadd_back(t_echo **lst, t_echo *new);
t_echo		*echo_lstnew(char *str, char c);
short		echo_lst_clear(t_echo *lst);
char		*set_pipe_line(char *s);
void		print_echo_quotes(t_echo **echo, t_data *data, int ret);
int			print_quote_ref(char *s, t_data *data);
short		is_echo_quotes(char *line);
void		sig_new(int sig);
int			s_s(char flag, int *res);
int			h_s(int flag, int *res);

#endif
