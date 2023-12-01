/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 14:22:12 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/30 14:22:13 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

static int	is_builtin(char *str)
{
	if (ft_equals(str, "pwd"))
		return (1);
	else if (ft_equals(str, "env"))
		return (1);
	else if (ft_equals(str, "exit"))
		return (1);
	else if (ft_equals(str, "echo"))
		return (1);
	else if (ft_equals(str, "export"))
		return (1);
	else if (ft_equals(str, "unset"))
		return (1);
	else if (ft_equals(str, "cd"))
		return (1);
	else
		return (0);
}

//TODO : Trouver la bonne taille pour le calloc
static char	**create_args(t_cmd *cmd)
{
	char	**ret;
	int		i;

	i = 0;
	ret = ft_calloc(10000000, sizeof(char));
	while (cmd->tokens->str)
	{
		ret[i] = ft_strdup(cmd->tokens->str);
		i++;
		cmd->tokens = cmd->tokens->next;
	}
	while (cmd->tokens->previous)
		cmd->tokens = cmd->tokens->previous;
	ret[i] = NULL;
	return (ret);
}

void	fork_returns(t_cmd *cmd, t_main **main, int pid)
{
	waitpid(pid, &((*main)->last), 0);
	(*main)->backupfd[0] = dup(0);
	(*main)->backupfd[1] = dup(1);
	if (cmd->pipe && (dup2((*main)->pipes[0], 0) == -1
			|| close((*main)->pipes[0]) == -1
			|| close((*main)->pipes[1]) == -1))
	{
		error_print(CRITICAL, "An error has occured while piping !", NULL);
		return ;
	}
	(*main)->state = 1;
}

int	exec_builtin(t_cmd *cmd, t_main **main)
{
	g_received_signal = IGNORE_NEW_LINE;
	(*main)->mode = 1;
	if (ft_equals(cmd->tokens->str, "pwd"))
		ft_pwd(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "env"))
		ft_env(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "exit"))
		ft_exit(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "echo"))
		ft_echo(cmd, (*main));
	else if (ft_equals(cmd->tokens->str, "export"))
		ft_export(cmd, main);
	else if (ft_equals(cmd->tokens->str, "unset"))
		ft_unset(cmd, main);
	else if (ft_equals(cmd->tokens->str, "cd"))
		ft_cd(cmd, main);
	if (g_received_signal == SIGNAL_QUIT || g_received_signal == SIGNAL_ABORT)
		return (-1);
	return (0);
}

int	is_system(t_cmd *cmd)
{
	if (ft_equals(cmd->tokens->str, "exit"))
		return (1);
	else if (ft_equals(cmd->tokens->str, "cd"))
		return (1);
	else if (ft_equals(cmd->tokens->str, "unset"))
		return (1);
	else if (ft_equals(cmd->tokens->str, "export") && cmd->tokens->next->str)
		return (1);
	return (0);
}

void	exec_general(t_cmd *cmd, t_main **main, int *ret)
{
	int		i;
	char	**paths;
	char	**envs;
	char	**args;
	char	*buff;

	i = 0;
	buff = ft_getenv((*main)->env, "PATH");
	paths = ft_split(buff, ':');
	free(buff);
	envs = env_to_array((*main)->env);
	args = create_args(cmd);
	while (paths[i])
	{
		buff = ft_calloc(1, sizeof(char));
		super_concat(&buff, paths[i]);
		super_concat(&buff, "/");
		super_concat(&buff, cmd->tokens->str);
		(*ret) = execve(buff, args, envs);
		free(buff);
		i++;
	}
	error_exec(errno);
	free_liste(envs);
	free_liste(paths);
	free_liste(args);
}

void	fork_core(t_cmd *cmd, t_main **main)
{
	int	ret;
	//int	codes[3];

	ret = 0;
	if ((*main)->state != LAST_PIPE)
	{
		if (dup2((*main)->pipes[1], STDOUT_FILENO) < 0)
			close((*main)->pipes[0]);
	}
	if (cmd->pipe && (dup2((*main)->pipes[1], 1) == -1
			|| close((*main)->pipes[0]) == -1
			|| close((*main)->pipes[1]) == -1))
	{
		error_print(CRITICAL, "An error has occured while piping !", NULL);
		exit(-1);
	}
	if (is_builtin(cmd->tokens->str))
		ret = exec_builtin(cmd, main);
	else
		exec_general(cmd, main, &ret);
	exit(ret);
}

void	ft_exec(t_cmd *cmd, t_main **main)
{
	int	pid;
	int	i;

	if (!cmd->pipe)
		(*main)->state = LAST_PIPE;
	i = pipe((*main)->pipes);
	if (cmd->pipe && i == -1)
	{
		error_print(CRITICAL, "An error has occured while piping !", NULL);
		return ;
	}
	if (is_system(cmd))
		exit(-3);
	else
	{
		pid = fork();
		if (pid < 0)
		{
			error_print(CRITICAL, "An error has occured while forking !", NULL);
			return ;
		}
		if (pid == 0)
			fork_core(cmd, main);
		else
			fork_returns(cmd, main, pid);
	}
	if (!cmd->pipe || g_received_signal == SIGNAL_QUIT
		|| g_received_signal == SIGNAL_ABORT)
		exit(0);
	cmd = cmd->pipe;
	(*main)->state = IN_PIPE;
	ft_exec(cmd, main);
}
