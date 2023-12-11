/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:13:11 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/03 08:13:12 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_received_signal = -1;

void	ft_eof2(t_main *main, int code)
{
	rl_clear_history();
	free(main->initpwd);
	free_env(main->env);
	free(main->to_parse);
	free_liste(main->iterate_liste);
	free_liste(main->pipe_liste);
	exit(code);
}

void	ft_eof(t_main *main)
{
	rl_clear_history();
	close_message();
	chdir(main->initpwd);
	free(main->initpwd);
	free_env(main->env);
	free(main->to_parse);
	exit(main->code_exit);
}

void	interrupt_sig(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_received_signal = SIGINT;
	}
	else if (sig == SIGSEGV)
		error_print(FUBAR, "Dump this core 🔪️", NULL);
	else
		g_received_signal = -1;
}

int	main(int args, char *argv[], char *env[])
{
	t_main	main;

	(void) args;
	(void) argv;
	welcome_message();
	signal(SIGINT, interrupt_sig);
	signal(SIGTSTP, interrupt_sig);
	signal(SIGQUIT, SIG_IGN);
	main.env = make_env(env);
	main.last = 0;
	main.fd[0] = dup(0);
	main.fd[1] = dup(1);
	main.code_exit = 0;
	main.initpwd = ft_getenv(main.env, "PWD");
	run(main);
}
