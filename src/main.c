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
	log_open_exit(1);
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
	ft_printf("#		😎️\x1b[31mPar Nolan MASCRIER et Martin JUETTE🤑️		  ");
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

void	ft_eof(t_main *main)
{
	(void)main;
	rl_clear_history();
	ft_printf("\x1b[31m\n\nGoodbye 💀️💀️💀️\n\x1b[0m ");
	log_open_exit(0);
	exit(0);
}

int	ft_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_whitespace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	main(int args, char *argv[], char *env[])
{
	char	*to_parse;

	(void) args;
	(void) argv;
	t_main	main;

	welcome_message();
	signal(SIGINT, interrupt_sig);
	signal(SIGTSTP, interrupt_sig);
	signal(SIGQUIT, SIG_IGN);
	main.env = make_env(env);
	main.last = 0;
	main.initpwd = ft_getenv(main.env, "PWD");
	while (1)
	{
		to_parse = readline("\x1b[37mminishell~$ ");
		if (!to_parse)
			ft_eof(&main);
		to_parse = check_quote(to_parse);
		if (!to_parse)
		{
			ft_printf("\n");
			continue ;
		}
		//if (!parse_error(to_parse))
		//	continue ;
		add_history(to_parse);
		if (to_parse && !ft_empty(to_parse))
			iterate(to_parse, &main);
		if (g_received_signal == -3)
			ft_eof(&main);
		g_received_signal = -1;
		free(to_parse);
	}
}
