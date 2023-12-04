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

void	welcome_message(void)
{
	ft_printf("\x1b[37m###########################################");
	ft_printf("########################\n");
	ft_printf("#				  				  #\n");
	ft_printf("#   \x1b[34m__  __ _____ _   _ _____  ");
	ft_printf("_____ _    _ ______ _      _      \x1b[37m  #\n");
	ft_printf("#  \x1b[34m|  \\/  |_   _| \\ | |_   _|");
	ft_printf("/ ____| |  | |  ____| |    | |     \x1b[37m  #\n");
	ft_printf("#  \x1b[34m| \\  / | | | |  \\| | | ");
	ft_printf("| | (___ | |__| | |__  | |    | |     \x1b[37m  #\n");
	ft_printf("#  \x1b[34m| |\\/| | | | | . ` | | |  \\_");
	ft_printf("__ \\|  __  |  __| | |    | |     \x1b[37m  #\n");
	ft_printf("#  \x1b[34m| |  | |_| |_| |\\  |_| |_");
	ft_printf(" ____) | |  | | |____| |____| |___  \x1b[37m  #\n");
	ft_printf("#  \x1b[34m|_|  |_|_____|_| \\_|_____");
	ft_printf("|_____/|_|  |_|______|______|______|\x1b[37m  #\n");
	ft_printf("#		  						  #\n");
	ft_printf("#        😎️\x1b[31mPar Nolan MASC");
	ft_printf("RIER et Martin JUETTE🤑️       		  ");
	ft_printf("\x1b[37m#\n##########################################");
	ft_printf("#########################\n\n");
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
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_received_signal = -3;
	}
}

int	main(int args, char *argv[], char *env[])
{
	char	*to_parse;
	char	*prompt;
	t_main	main;

	(void) args;
	(void) argv;
	to_parse = NULL;
	prompt = NULL;
	welcome_message();
	signal(SIGINT, interrupt_sig);
	signal(SIGTSTP, interrupt_sig);
	signal(SIGQUIT, SIG_IGN);
	main.env = make_env(env);
	main.last = 0;
	main.backupfd[0] = dup(0);
	main.backupfd[1] = dup(1);
	main.initpwd = ft_getenv(main.env, "PWD");
	run(to_parse, prompt, main);
}
