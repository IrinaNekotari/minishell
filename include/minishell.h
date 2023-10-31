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
# include <fcntl.h>
# include <errno.h>

# define ERR_CMD_NOT_FOUND "minishell: command not found:"
# define ERR_TOO_MANY_ARGS ": too many arguments"

# define LOGGER 1
# define LOG_TO_FILE 0
# define FALSE 0
# define TRUE 1

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
	struct s_word	*tokens;
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
void	parse_single_2(char *s, t_word *c);
void	parse_with_pipes(char **t, t_cmd *c);
void	ft_trimword(t_word *c);
void	free_command(t_cmd *c);
void	free_liste(char **lst);
void	log_input(char	*str);
void	log_parse_single(char *str);
void	log_open_exit(int i);
char	*ft_concat(char *s1, char *s2);
char	*add_quote(char *s, char quote);
char	*check_quote(char *s);
int	is_whitespace(char c);
int	parse_error(char *str);
int	is_escapable(char c);
int	is_escapable2(char c);
int	is_blank(char *ptr, int j);
void	debug_show_command(t_word *t);
void	debug_show_all(t_cmd *c);
void	execute(t_cmd *cmd, char **env);
int	is_blank(char *ptr, int j);
char	**counter_split(char *s, char **to_ret);
char	**split_semicolon(char *s, char **to_ret);

#endif
