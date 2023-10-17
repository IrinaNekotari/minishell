/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 08:13:30 by nmascrie          #+#    #+#             */
/*   Updated: 2023/10/03 08:13:31 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdbool.h>
# include <stdint.h>
# include <sys/types.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define ERR_CMD_NOT_FOUND "minishell: command not found:"
# define ERR_TOO_MANY_ARGS ": too many arguments"

/*
* Stocke les donnees pour les redirections < > << >>
*/
typedef struct s_redir
{
	int	fd;
	char	*file;
}	t_redir;

/*
* Stocke un "mot" et stocke s'il est quote par des '' "" ou \
*/
typedef struct	s_word
{
	char		*str;
	char		quote;
	struct s_word	*next;
}	t_word;
/*
* Stocke les valeurs d'environnement
*/
typedef struct	s_env
{
	char	**env;
	char	*oldpwd;
}	t_env;
typedef struct	s_var
{
	bool		exit;
	bool		is_a_tty;
	uint8_t	return_value;
}	t_var;

/*
* Stocke les donnees de la commande
*/
typedef struct s_cmd
{
	char	**cmd;
	int	ret;
	int	pid;
	struct s_redir	input;
	struct s_redir	output;
	struct s_cmd	*pipe;
}	t_cmd;

/*
*	Structure principale pour ballader les valeurs d'env, de val, et la derniere commande
*	enregistree
*/
typedef struct	s_main
{
	char	*prompt;
	t_env	env;
	t_var	var;
}	t_main;

extern int	g_received_signal;

void	parse(char *s, char **env);
char	*check_quote(char *s);
void	iterate(char *s, char **env);
void	parse_single(char *s, t_word *c);
void	parse_with_pipes(char **s, t_word *c);

#endif
