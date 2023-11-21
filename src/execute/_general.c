#include "minishell.h"

static int	good_size(t_cmd *cmd)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (cmd->tokens->str)
	{
		i++;
		if (len < (int)ft_strlen(cmd->tokens->str))
			len = ft_strlen(cmd->tokens->str);
		cmd->tokens = cmd->tokens->next;
	}
	while (cmd->tokens->previous)
		cmd->tokens = cmd->tokens->previous;
	return ((i + 1) * (len + 1));
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
	while (cmd->tokens->previous)
		cmd->tokens = cmd->tokens->previous;
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
void	exe(char *path, char *file, char ** args, t_main **main)
{
	int		pid;
	int		ret;
	int		i;
	char	*try;
	char	**paths;

	ret = SUCCESS;
	pid = fork();
	paths = ft_split(path, ':');
	if (pid == 0)
	{
		i = 0;
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
		waitpid(pid, &ret, 0);
	free_liste(paths);
	(*main)->last = ret;
}

void	execute_general(t_cmd *cmd, t_main *main)
{
	char		*path;
	char		*file;
	char	**args;

	path = ft_getenv(main->env, "PATH");
	if (!path)
	{
		return ;
	}
	file = ft_strdup(cmd->tokens->str);
	args = create_args(cmd);
	exe(path, file, args, &main);
	if (args)
		free_liste(args);
	free(path);
	free(file);
}
