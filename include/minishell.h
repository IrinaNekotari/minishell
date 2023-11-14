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

# define LOGGER 0
# define LOG_TO_FILE 0
# define FALSE 0
# define TRUE 1

# define SINGLE_OUTPUT 1
# define DOUBLE_OUTPUT 2
# define SINGLE_INPUT 3
# define DOUBLE_INPUT 4

/*
* Stocke les donnees pour les redirections < > << >>
*/
typedef struct s_io
{
	int	io;
	char	*file;
	struct s_io	*next;
	struct s_io	*previous;
}	t_io;

/*
* Stocke un "mot" et stocke s'il est quote par des '' "" ou \
*/
typedef struct	s_word
{
	char		*str;
	char		quote;
	struct s_word	*next;
	struct s_word	*previous;
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

/**
* Une liste chainee basique utilisee dans les verifs io
*/
typedef struct s_pile
{
	char	*str;
	struct s_pile	*next;
	struct s_pile	*previous;
}	t_pile;

/*
* Stocke les donnees de la commande
*/
typedef struct s_cmd
{
	struct s_word	*tokens;
	struct s_io *input;
	struct s_io *output;
	struct s_cmd	*pipe;
	struct s_cmd	*previous;
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
void	execute_general(t_cmd *cmd, char **env);
int	chain_as_equals(t_cmd **cmd, char *cmp);
int	ft_equals(char *s1, char *s2);
void	ft_pwd(t_cmd *cmd, char **env);
void	ft_export(t_cmd *cmd, char **env);
void	ft_unset(t_cmd *cmd, char **env);
void	ft_echo(t_cmd *cmd, char **env);
void	ft_env(t_cmd *cmd, char **env);
void	ft_exit(t_cmd *cmd, char **env);
int	ft_equals(char *s1, char *s2);
void	generate_io(t_cmd **cmd);
void	rollback_tokens(t_cmd **cmd);
void	rollback_io(t_cmd **cmd);
void	rollback_cmd(t_cmd **cmd);
void	print_io(t_cmd *cmd, char *str);
void	handle_output(t_cmd *cmd, char *str);
int	handle_input(t_cmd *cmd);
void	handle_output_create(t_cmd *cmd);
char	*ft_append(char *str, char c);

#endif
