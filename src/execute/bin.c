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

int	fork_returns(t_cmd *cmd, t_main **main, int pid)
{
	int	exit_val;
	
	exit_val = 0;
	waitpid(pid, &exit_val, 0);
	if (cmd->pipe && (dup2((*main)->pipes[0], 0) == -1
			|| close((*main)->pipes[0]) == -1
			|| close((*main)->pipes[1]) == -1))
	{
		error_print(CRITICAL, "An error has occured while piping !", NULL);
		return (exit_val);
	}
	(*main)->state = 1;
	return (exit_val);
}

void	fork_core(t_cmd *cmd, t_main **main)
{
	int	ret;

	ret = 0;
	//TODO : Ttrouver le bon signal ?
	signal(SIGINT, SIG_DFL);
	if (cmd->output->file)
		io_pipe(cmd, main);
	else if (cmd->input->file)
		io_pipe2(cmd, main);
	else if (cmd->pipe && (dup2((*main)->pipes[1], 1) == -1
			|| close((*main)->pipes[0]) == -1
			|| close((*main)->pipes[1]) == -1))
	{
		error_print(CRITICAL, "An error has occured while piping !", NULL);
		exit(-1);
	}
	if (is_system(cmd))
		ft_printf("");
	else if (is_builtin(cmd->tokens->str))
		ret = exec_builtin(cmd, main);
	else
		ret = exec_general(cmd, main, &ret);
	exit(ret);
}

int	exec_general(t_cmd *cmd, t_main **main, int *ret)
{
	int		i;
	char	**paths;
	char	**envs;
	char	**args;
	char	*buff;

	envs = env_to_array((*main)->env);
	args = create_args(cmd);
	i = 0;
	if (cmd->tokens->str[0] == '/')
	{
		*ret = execve(cmd->tokens->str, args, envs);
	}
	else
	{
		buff = ft_getenv((*main)->env, "PATH");
		paths = ft_split(buff, ':');
		free(buff);
		while (paths[i])
		{
			buff = ft_calloc(1, sizeof(char));
			super_concat(&buff, paths[i]);
			super_concat(&buff, "/");
			super_concat(&buff, cmd->tokens->str);
			*ret = execve(buff, args, envs);
			free(buff);
			i++;
		}
		free_liste(paths);
	}
	error_exec(errno);
	free_liste(envs);
	free_liste(args);
	exit((*ret));
}

void	ft_exec(t_cmd *cmd, t_main **main)
{
	int	pid;
	int	i;
	int	exit_val;
	
	if (!cmd->pipe)
		(*main)->state = LAST_PIPE;
	i = pipe((*main)->pipes);
	if (cmd->pipe && i == -1)
	{
		error_print(CRITICAL, "An error has occured while piping !", NULL);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		error_print(CRITICAL, "An error has occured while forking !", NULL);
		return ;
	}
	if (pid == 0)
		fork_core(cmd, main);
	else
		exit_val = fork_returns(cmd, main, pid);
	//Affiche le BON code ici, donc ça veut dire que l'info remonte bien jusque la
	//Pourquoi l'exit n'envoie pas le bon code alors ?
	//ft_putstr_fd(ft_itoa(exit_val), 2);
	//printf("parent get: %d of child\n", WEXITSTATUS((*main)->last));
	//ft_putstr_fd("\n", 2);
	if (!cmd->pipe || g_received_signal == SIGNAL_QUIT
		|| g_received_signal == SIGNAL_ABORT)
		exit(exit_val);
	cmd = cmd->pipe;
	(*main)->state = IN_PIPE;
	ft_exec(cmd, main);
}
