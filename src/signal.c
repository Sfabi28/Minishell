/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stescaro <stescaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:27:31 by stescaro          #+#    #+#             */
/*   Updated: 2024/05/09 15:21:34 by stescaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int signum)
{
	printf("\n");
	if (signum == SIGQUIT)
		g_ctrl = 131;
	else if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_ctrl = 130;
	}
}

void	sig_new(int sig)
{
	if (sig == SIGINT)
	{
		g_ctrl = 130;
		printf("\n");
		signal(SIGINT, SIG_IGN);
	}
	else if (sig == SIGQUIT)
	{
		g_ctrl = 131;
		printf("Quit (core dumped)\n");
		signal(SIGQUIT, SIG_IGN);
	}
}

int	s_s(char flag, int *res)
{
	if (flag == 1)
	{
		signal(SIGINT, sig_new);
		signal(SIGQUIT, sig_new);
	}
	else if (flag == 2)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
		if (g_ctrl == 130)
			*res = 130;
		else if (g_ctrl == 131)
			*res = 131;
	}
	return (1);
}

int	h_s(int flag, int *res)
{
	s_s(flag, res);
	write(2, "minishell: warning: heredoc delimited by end-of-file\n", 53);
	return (1);
}
