/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:36:26 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/03 08:36:27 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_occur(char *str, char c)
{
	int	i;
	int	count;
	
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			if ((i >= 1 && str[i - 1] != '\\') || i == 0)
				count++;
		}
		i++;
	}
	return (count);
}

void	parse(char *s, char **env)
{
	t_cmd	*cmd;
	char	**t;

	cmd = ft_calloc(2, sizeof(t_cmd));
	cmd->tokens = ft_calloc(2, sizeof(t_word));
	t = (char **) ft_calloc(count_occur(s, '|') + 1, sizeof(int) * 100);
	t = counter_split(s, t);
	if (ft_strchr(s, '|'))
		parse_with_pipes(t, cmd);
	else
	{
		cmd->pipe = NULL;
		parse_single_2(t[0], cmd->tokens);
	}
	generate_io(cmd);
	execute(cmd, env);
	free_liste(t);
	free_command(cmd);
}

void	parse_with_pipes(char **t, t_cmd *c)
{
	int		i;
	t_cmd	*c_bck;

	c_bck = c;
	i = 0;
	while (t[i])
	{
		parse_single_2(t[i], c_bck->tokens);
		if (t[i + 1])
		{
			c_bck->pipe = ft_calloc(2, sizeof(t_cmd));
			c_bck = c_bck->pipe;
			c_bck->tokens = ft_calloc(2, sizeof(t_word));
		}
		else
			c_bck->pipe = NULL;
		i++;
	}
}

void	iterate(char *s, char **env)
{
	char	**lst;
	int		i;

	i = 0;

	//TODO : Chercher la bonne taille
	lst = (char **) ft_calloc(count_occur(s, ';') + 1, sizeof(int) * 100);
	lst = split_semicolon(s, lst);
	log_input(s);
	free(s);
	while (lst[i])
	{
		parse(lst[i], env);
		i++;
	}
	free_liste(lst);
}
