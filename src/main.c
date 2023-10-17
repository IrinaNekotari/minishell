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
	ft_printf("#		\x1b[31mPar Nolan MASCRIER et Martin JUETTE		  ");
	ft_printf("\x1b[37m#\n##########################################");
	ft_printf("#########################\n");
}

void	interrupt_sig(int sig)
{
	if (sig == SIGINT)
	{
		//TODO : Doit afficher un nouveau prompt
		ft_printf("\x1b[31m\nTerminating the minishell with ");
		ft_printf("\x1b[37mCTRL + C \x1b[31m...");
	}
	else
	{
		ft_printf("\x1b[31m\nTerminating the minishell with ");
		ft_printf("\x1b[37mCTRL + Z \x1b[31m...");
		/*TODO : Tous les free et compagnie**/
		ft_printf("\x1b[31m\nGoodbye");
		exit(0);
	}
}

void	ft_eof(void)
{
	ft_printf("\x1b[31m\nTerminating the minishell with ");
	ft_printf("\x1b[37mCTRL + D (EoF) \x1b[31m...");
	/*TODO : Tous les free et compagnie*/
	ft_printf("\x1b[31m\nGoodbye");
	exit(0);
}

int	main(int args, char *argv[], char *env[])
{
	char	*to_parse;

	(void) args;
	(void) argv;
	welcome_message();
	signal(SIGINT, interrupt_sig);
	signal(SIGTSTP, interrupt_sig);
	signal(SIGQUIT, SIG_IGN);
	to_parse = readline("\n~$ ");
	if (!to_parse)
		ft_eof();
	while (to_parse)
	{
		to_parse = check_quote(to_parse);
		add_history(to_parse);
		iterate(to_parse, env);
		free(to_parse);
		to_parse = readline("~$ ");
		if (!to_parse)
			ft_eof();
	}
}
