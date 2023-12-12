/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 15:08:48 by mjuette           #+#    #+#             */
/*   Updated: 2023/12/04 15:08:51 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	close_message(void)
{
	ft_printf("\n\x1b[37m###########################################");
	ft_printf("########################\n");
	ft_printf("#\x1b[1m\x1b[44m         ");
	ft_printf("            __  __  __  __  __     __                   ");
	ft_printf("\x1b[0m\x1b[37m#\n");
	ft_printf("#\x1b[1m\x1b[44m                    ");
	ft_printf("/ _ /  \\/  \\|  \\|__)\\_/|_       ");
	ft_printf("o/    o/     \x1b[0m\x1b[37m#\n");
	ft_printf("#\x1b[1m\x1b[44m                    ");
	ft_printf("\\__)\\__/\\__/|__/|__) | |__    ");
	ft_printf("/(.)   /|      \x1b[0m\x1b[37m#\n");
	ft_printf("#\x1b[1m\x1b[44m                   ");
	ft_printf("                               ");
	ft_printf(" / \\    /\\     \x1b[0m\x1b[37m#\n");
	ft_printf("\x1b[37m###########################################");
	ft_printf("########################\n\x1b[0m");
}

char	*get_prompt(t_main *main)
{
	char	*temp;
	char	*prompt;

	temp = ft_getenv(main->env, "PWD");
	if (!temp)
		prompt = ft_strdup("[\x1b[35m???\x1b[37m]");
	else if (ft_equals(main->initpwd, temp))
		prompt = ft_strdup("[\x1b[36mHome\x1b[37m]");
	else
	{
		prompt = ft_calloc(1, sizeof(char));
		super_concat(&prompt, "[\x1b[33m");
		free(temp);
		temp = ft_getenv(main->env, "PWD");
		super_concat(&prompt, temp);
		super_concat(&prompt, "\x1b[37m]");
	}
	if (temp)
		free(temp);
	super_concat(&prompt, " minishell :: ");
	return (prompt);
}

void	run(t_main main)
{
	while (1)
	{
		dup2(main.fd[0], 0);
		main.prompt = get_prompt(&main);
		main.to_parse = readline(main.prompt);
		main.state = 0;
		free(main.prompt);
		if (!main.to_parse && main.state != 1)
			ft_eof(&main);
		main.state = 0;
		main.to_parse = check_quote(main.to_parse);
		main.to_parse = check_pipes(main.to_parse);
		if (!main.to_parse)
			continue ;
		if (main.to_parse && !ft_empty(main.to_parse))
			iterate(main.to_parse, &main);
		if (g_received_signal == SIGNAL_QUIT)
			ft_eof(&main);
		g_received_signal = -1;
		free(main.to_parse);
	}
}
