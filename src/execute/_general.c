#include "minishell.h"

//TODO : Trouver la bonne taille pour le calloc
char	**create_args(t_cmd *cmd)
{
	char	**ret;
	int		i;

	i = 0;
	cmd->tokens = cmd->tokens->next;
	while (cmd->tokens->str)
	{
		i++;
		cmd->tokens = cmd->tokens->next;
	}
	ret = ft_calloc(i*100, sizeof(char));
	ret[i--] = NULL;
	cmd->tokens = cmd->tokens->previous;
	while (cmd->tokens->previous && i >= 0)
	{
		ret[i--] = ft_strdup(cmd->tokens->str);
		cmd->tokens = cmd->tokens->previous;
	}
	return (ret);
}

void	exe(char *path, char **args, t_cmd *cmd, t_env *env)
{
	int	pid;
	int	ret;

	ret = SUCCESS;
	(void)cmd;
	(void)env;
	(void)args;
	(void)path;
	pid = fork();
	if (pid == 0)
	{
		//execve(path, args, env);
		ft_printf("BOOOM!!!!\n");
		exit(ret);
	}
	else
	{
		waitpid(pid, &ret, 0);
		ft_printf("WOOHOO!!!! %d \n", ret);
	}
}

void	execute_general(t_cmd *cmd, t_main *main)
{
	/*char	**args;
	
	args = create_args(cmd);
	exe(cmd->tokens->str, args, cmd, main->env);
	free_liste(args);*/
	(void)main;
	debug_show_all(cmd);

}
