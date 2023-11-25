#include "minishell.h"

static int	good_size(t_cmd *cmd)
{
	int	i;

	i = 0;
	i += tokens_length(cmd) * tokens_depth(cmd);
	i += 1;
	return (i);
}

//TODO : Trouver la bonne taille pour le calloc
char	**create_args(t_cmd *cmd)
{
	char	**ret;
	int		i;

	i = 0;
	ret = ft_calloc(good_size(cmd)*100, sizeof(char));
	while (cmd->tokens->str)
	{
		ret[i] = ft_strdup(cmd->tokens->str);
		i++;
		cmd->tokens = cmd->tokens->next;
	}
	ret[i] = NULL;
	return (ret);
}

static int	exec(char *try, char **args, t_main **main)
{
	char	**env;
	int		ret;
	env = env_to_array((*main)->env);
	ret = execve(try, args, env);
	free_liste(env);
	return (ret);
}

//TODO : Gestion des erreurs
//JAMAIS on arrive a norminer cet étron fumant
char	*exe(char *path, char *file, char ** args, t_main **main)
{
	int		pid;
	int		ret;
	int		i;
	char	*try;
	char	**paths;
	int	*pipes;
	char	*try2;

	pipes = ft_calloc(2, sizeof(int));
	pipe(pipes);
	//TODO : Gerer les erreurs ?
	try2 = ft_calloc(100000, sizeof(char));
	ret = SUCCESS;
	pid = fork();
	paths = ft_split(path, ':');
	if (pid == 0)
	{
		i = 0;
		close(pipes[0]);
		dup2(pipes[1], 1);
		while (paths[i])
		{
			try = ft_calloc(1, sizeof(char));
			super_concat(&try, paths[i]);
			super_concat(&try, "/");
			super_concat(&try, file);
			ret = exec(try, args, main);
			free(try);
			i++;
		}
		error_exec(errno);
		exit(ret);
	}
	else
	{
		waitpid(pid, &ret, 0);
		close(pipes[1]);
		i = 1;
		while (i)
			i = read(pipes[0], try2, 1023);
	}
	free_liste(paths);
	(*main)->last = ret;
	free(pipes);
	return (try2);
}

void	execute_general(t_cmd *cmd, t_main *main)
{
	char		*path;
	char		*file;
	char		*write;
	char	**args;

	path = ft_getenv(main->env, "PATH");
	if (!path)
	{
		return ;
	}
	file = ft_strdup(cmd->tokens->str);
	args = create_args(cmd);
	write = exe(path, file, args, &main);
	if (args)
		free_liste(args);
	free(path);
	free(file);
	print_io2(cmd, write);
	free(write);
}
