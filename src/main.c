/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 11:20:15 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:01 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_ctrl = 0;

int	header(void)
{
	printf("\033[36m/-----------------------------------------------------");
	printf("-------------------------\\\n|                                  ");
	printf("                                            |\n|  ||\\  /||  || ");
	printf(" ||\\   ||  ||   ///////  ||    ||  |//////|  ||       ||       |");
	printf("\n|  ||\\\\//||  ||  ||\\\\  ||  ||  //        ||    ||  ||     ");
	printf("   ||       ||       |\n|  || \\/ ||  ||  || \\\\ ||  ||  \\\\");
	printf("\\\\\\\\\\   |//////|  |///|     ||       ||       |\n|  ||    ");
	printf("||  ||  ||  \\\\||  ||        //  ||    ||  ||        ||      ");
	printf(" ||       |\n|  ||    ||  ||  ||   \\||  ||  ///////   ||    ||");
	printf("  |\\\\\\\\\\\\|  |\\\\\\\\\\|  |\\\\\\\\\\|  |\n|             ");
	printf("                                                                 ");
	printf("|\n\\-----------------------------------------------------------");
	printf("-------------------/\n\033[0m\n");
	return (0);
}

static	t_env	*set_envlist(char **env)
{
	t_env	*envlist;
	t_env	*temp;
	int		i;

	i = 0;
	envlist = NULL;
	while (env[i])
	{
		temp = env_lstnew(env[i], env);
		if (!temp)
			return (env_lstclear(envlist), NULL);
		env_lstadd_back(&envlist, temp);
		i++;
	}
	return (envlist);
}

static	t_line	*set_line(t_memory *memory, t_env *env, t_echo **echo)
{
	char		*l;
	const char	*prompt;
	t_line		*line;

	prompt = prompt_location(env);
	l = readline(prompt);
	free((char *)prompt);
	if (!l)
	{
		printf("exit\n");
		return (NULL);
	}
	line = line_lstnew(l, memory, env, echo);
	add_history(l);
	free(l);
	return (line);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		ret;

	(void)((argc) + (argv));
	data.env = set_envlist(env);
	ret = WEXITSTATUS(ft_execve("clear", data.env, 0));
	data.memory = NULL;
	data.echo = NULL;
	while (ret > -9)
	{
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
		data.line = set_line(data.memory, data.env, &(data.echo));
		if (!data.line && env_lstclear(data.env) && echo_lst_clear(data.echo))
			return (memory_lstclear(data.memory), clear_history(), 0);
		ret = minishell(&data, data.line, ret + g_ctrl
				* (g_ctrl > 129 && g_ctrl < 132), -10);
		if (g_ctrl)
			g_ctrl = 0;
		line_lstclear(data.line);
	}
	(void)(env_lstclear(data.env) + echo_lst_clear(data.echo));
	memory_lstclear(data.memory);
	return (clear_history(), -10 - ret);
}
