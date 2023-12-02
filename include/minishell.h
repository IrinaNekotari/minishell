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
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>

# define ERR_CMD_NOT_FOUND "minishell: command not found:"
# define ERR_TOO_MANY_ARGS ": too many arguments"

# define SUCCESS 0

# define LOGGER 0
# define LOG_TO_FILE 0
# define FALSE 0
# define TRUE 1

# define SINGLE_OUTPUT 1
# define DOUBLE_OUTPUT 2
# define SINGLE_INPUT 3
# define DOUBLE_INPUT 4

# define INFO 0
# define WARNING 1
# define ERROR 2
# define CRITICAL 3
# define FUBAR 4

# define FIRST_PIPE 0
# define LAST_PIPE 2
# define IN_PIPE 1

# define IGNORE_NEW_LINE -8
# define SIGNAL_QUIT -3
# define SIGNAL_ABORT -5
# define IGNORE_PIPES -69

/*
* Suhz-Mahbyt la norminette
*/
typedef struct s_norme
{
	int	a;
	int	b;
	int	c;
	int	d;
	int	e;
	int	f;
	int	g;
	int	h;
	int	i;
	int	j;
	int	k;
	int	l;
	int	m;
	int	n;
	int	o;
	int	p;
	int	q;
	int	r;
	int	s;
	int	t;
	int	u;
	int	v;
	int	w;
	int	x;
	int	y;
	int	z;
}	t_norme;

/*
* Stocke les donnees pour les redirections < > << >>
*/
typedef struct s_io
{
	int			io;
	char		*file;
	struct s_io	*next;
	struct s_io	*previous;
}	t_io;

/*
* Stocke un "mot" et stocke s'il est quote par des '' "" ou \
*/
typedef struct s_word
{
	char			quote;
	char			*str;
	struct s_word	*next;
	struct s_word	*previous;
}	t_word;
/*
* Stocke les valeurs d'environnement
*/
typedef struct s_env
{
	struct s_env		*next;
	struct s_env		*previous;
	char				*name;
	char				*value;
}	t_env;

/**
* Une liste chainee basique utilisee dans les verifs io
*/
typedef struct s_pile
{
	char			*str;
	struct s_pile	*next;
	struct s_pile	*previous;
}	t_pile;

/*
* Stocke les donnees de la commande
*/
typedef struct s_cmd
{
	struct s_io		*input;
	struct s_io		*output;
	struct s_word	*tokens;
	struct s_cmd	*pipe;
	struct s_cmd	*previous;
	int				*pipes;
}	t_cmd;

/*
*	Structure principale pour ballader les valeurs d'env, 
*	de val, et la derniere commande
*	enregistree
*/
typedef struct s_main
{
	int		last;
	int		pipes[2];
	int		backupfd[2];
	int		mode;
	int		state;
	char	*initpwd;
	t_env	*env;
}	t_main;

extern int	g_received_signal;

int		parse_error(char *str);
int		str_env_len(char **env);
int		ft_equals(char *s1, char *s2);
int		ft_empty(char *str);
int		handle_input(t_cmd *cmd);
int		is_blank(char *ptr, int j);
int		is_builtin(char *str);
int		is_delim(char c);
int		is_escapable(char c);
int		is_escapable2(char c);
int		is_system(t_cmd *cmd);
int		is_usable(char c);
int		is_whitespace(char c);
int		error_syntax(int severity, char *loc);
int		check_chevrons(t_cmd **cmd);
int		exec_builtin(t_cmd *cmd, t_main **main);
int		input_length(t_cmd *cmd);
int		output_length(t_cmd *cmd);
int		tokens_length(t_cmd *cmd);
int		env_length(t_env *env);
int		input_depth(t_cmd *cmd);
int		output_depth(t_cmd *cmd);
int		tokens_depth(t_cmd *cmd);
int		env_depth(t_env *env);
int		cmd_depth(t_cmd *cmd);

/*
*	Builtins et execution
*/
void	ft_pwd(t_cmd *cmd, t_main *main);
void	ft_export(t_cmd *cmd, t_main **main);
void	ft_unset(t_cmd *cmd, t_main **main);
void	ft_echo(t_cmd *cmd, t_main *main);
void	ft_cd(t_cmd *cmd, t_main **main);
void	ft_env(t_cmd *cmd, t_main *main);
void	ft_exit(t_cmd *cmd, t_main *main);
void	execute_general(t_cmd *cmd, t_main *main);
void	ft_exec(t_cmd *cmd, t_main **main);

void	parse(char *s, t_main **main);
void	iterate(char *s, t_main *main);
void	parse_single(char *s, t_cmd **cmd);
void	parse_single_2(char *s, t_word *c);
void	parse_with_pipes(char **t, t_cmd **c);
void	ft_trimword(t_word *c);
void	free_command(t_cmd *c);
void	free_liste(char **lst);
void	log_input(char	*str);
void	generate_variables(t_cmd **cmd, t_main **main);
void	log_parse_single(char *str);
void	log_open_exit(int i);
void	debug_show_command(t_word *t);
void	debug_show_all(t_cmd *c);
void	execute(t_cmd *cmd, t_main **main);
void	sort_env(char **tab, int env_len);
char	*print_sorted_env(t_env *env);
void	generate_io(t_cmd **cmd);
void	update_env(t_env **env, char *name, char *newvalue);
void	rollback_tokens(t_cmd **cmd);
void	rollback_io(t_cmd **cmd);
void	rollback_cmd(t_cmd **cmd);
void	print_io(t_cmd *cmd, char *str, t_main **main);
void	handle_output(t_cmd *cmd, char *str);
void	handle_output_create(t_cmd *cmd);
void	rollback_env(t_env **env);
void	ft_eof(t_main *main);
void	add_to_env(t_env **env, char *name, char *value);
void	del_from_env(t_env **env, char *name);
void	free_env(t_env *env);
void	super_concat(char **a, char *b);
void	hyper_concat(char **base, ...);
void	generate_env(char *env, char **name, char **value);
void	error_exec(int err);
void	error_print(int severerity, char *msg, char *add);

char	*check_quote(char *s);
char	*ft_concat(char *s1, char *s2);
char	*add_quote(char *s, char quote);
char	*check_quote(char *s);
char	*env_to_str(t_env *lst);
char	*ft_append(char *str, char c);
char	*ft_getenv(t_env *env, char *search);
char	*ft_concat2(char *s1, char *s2);
char	*check_pipes(char *s);

char	**counter_split(char *s, char **to_ret);
char	**split_semicolon(char *s, char **to_ret);
char	**env_to_array(t_env *env);

t_env	*make_env(char **env);

#endif
