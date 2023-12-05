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

void	ft_eof(t_main *main)
{
	(void)main;
	rl_clear_history();
	ft_printf("\x1b[31m\n\nGoodbye 💀️💀️💀️\x1b[0m");
	chdir(main->initpwd);
	exit(0);
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

void	run(char *to_parse, char *prompt, t_main main)
{
	while (1)
	{
		dup2(main.fd[0], 0);
		prompt = get_prompt(&main);
		to_parse = readline(prompt);
		main.state = 0;
		free(prompt);
		if (!to_parse && main.state != 1)
			ft_eof(&main);
		main.state = 0;
		to_parse = check_quote(to_parse);
		to_parse = check_pipes(to_parse);
		if (!to_parse)
			continue ;
		if (to_parse && !ft_empty(to_parse))
			iterate(to_parse, &main);
		if (g_received_signal == SIGNAL_QUIT)
			ft_eof(&main);
		g_received_signal = -1;
		free(to_parse);
		if (main.state == LAST_PIPE)
			exit(0);
	}
}
