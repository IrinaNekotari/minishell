/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_forks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 11:51:26 by nmascrie          #+#    #+#             */
/*   Updated: 2023/12/05 11:51:27 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_received_signal;

static char	**create_args(t_cmd *cmd, t_main **main)
{
	char	**ret;
	int		i;

	i = 0;
	ret = ft_calloc(2000000, sizeof(char));
	if (!ret)
	{
		error_print(CRITICAL, "An error has occured while allocating !", NULL);
		ft_eof2((*main), SIGNAL_ABORT);
	}
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

void	ft_execve(t_main **main, char **envs, char **args, int *ret)
{
	char	*buff;
	char	**paths;
	int		i;

	i = 0;
	buff = ft_getenv((*main)->env, "PATH");
	if (!buff)
	{
		error_print(ERROR, "PATH not found !", NULL);
		ft_eof2((*main), -1);
	}
	paths = ft_split(buff, ':');
	free(buff);
	while (paths[i])
	{
		buff = ft_calloc(1, sizeof(char));
		ultra_concat(&buff, paths[i], "/", args[0]);
		(*ret) = execve(buff, args, envs);
		free(buff);
		i++;
	}
	(*ret) = execve(args[0], args, envs);
	free_liste(paths);
}

int	exec_general(t_cmd *cmd, t_main **main, int *ret)
{
	char	**envs;
	char	**args;

	signal(SIGINT, SIG_DFL);
	envs = env_to_array((*main)->env);
	args = create_args(cmd, main);
	if (cmd->tokens->str[0] == '/')
		(*ret) = execve(cmd->tokens->str, args, envs);
	else
		ft_execve(main, envs, args, ret);
	error_exec(errno);
	free_liste(envs);
	free_liste(args);
	return (errno);
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
		ft_eof2((*main), -1);
	}
	if (is_system(cmd))
		ft_printf("");
	else if (is_builtin(cmd->tokens->str))
		ret = exec_builtin(cmd, main);
	else
		ret = exec_general(cmd, main, &ret);
	free_command(cmd);
	ft_eof2((*main), ret);
}
