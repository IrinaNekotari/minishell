#include "minishell.h"

extern int	g_received_signal;

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
//J'ai suivi ce tuto : https://www.mbillaud.fr/notes/pipeline.html
char	*exe(char *path, char *file, char ** args, t_main **main)
{
	int		pid;
	int		ret;
	int		i;
	char	*try;
	char	**paths;
	//static int	*pipes;
	char	*try2;

	//pipes = ft_calloc(2, sizeof(int));
	//pipe(pipes);
	//TODO : Gerer les erreurs ?
	try2 = ft_calloc(100000, sizeof(char));
	ret = SUCCESS;
	pid = fork();
	paths = ft_split(path, ':');
	if (pid == 0)
	{
		i = 0;
		//Cas général
		if ((*main)->state == 1)
		{
			//ft_putstr_fd((*main)->inpipe, (*main)->pipes2[0]);
			dup2((*main)->pipes2[1], STDIN_FILENO);
			//dup2(STDIN_FILENO, (*main)->pipes[0]);
		}
		dup2((*main)->pipes[1], STDOUT_FILENO);
		//dup2((*main)->pipes[1], (*main)->pipes2[1]);
		//dup2(STDOUT_FILENO, (*main)->pipes[1]);
		//close((*main)->pipes[0]);
		close((*main)->pipes[1]);
		//close((*main)->pipes2[0]);
		//close((*main)->pipes2[1]);
		//ft_putstr_fd((*main)->inpipe, 1);
		//close(0);
		//dup2(pipes[0], 0);
		//MARTIN
		//Demande aux autres comment nous sortir de la
		//Comment on est censé envoyer le résultat de la pipe
		//précédante 
		//Au besoin, elle est stockée dans
		// (*main)->inpipe en char *
		//Dans l'entrée de la commande actuelle
		//J'ai essayé plein de trucs, mais rien trouvé
		//ça me hante en pleine nuit ces conneries
		//Les pipes sont ouvertes dans l'execute
		while (paths[i])
		{
			//Ce que fait ce morceau : pour chaque variable
			//PATH, ça concatène le PATH avec un / et le
			//Nom de l'executable (premier token), 
			//Et tente de l'executer
			try = ft_calloc(1, sizeof(char));
			super_concat(&try, paths[i]);
			super_concat(&try, "/");
			super_concat(&try, file);
			ret = exec(try, args, main);
			free(try);
			i++;
		}
		//Si jamais toutes les executions ont échoués
		//On affiche l'erreur et on retourne le code d'erreur
		error_exec(errno);
		exit(ret);
	}
	//La, on attends que le fork se finisse
	waitpid(pid, &ret, 0);
	dup2((*main)->pipes[0], (*main)->pipes2[1]);
	close((*main)->pipes[1]);
	//On reconstitue le retour (sortie standard)
	//Dans le char * try
	//Qu'on retourne a la fin
	i = 1;
	while (i)
		i = read((*main)->pipes[0], try2, 1023);
	free_liste(paths);
	(*main)->last = ret;
	ft_putstr_fd(try2, (*main)->pipes2[1]);
	//free(pipes);
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
	g_received_signal = IGNORE_NEW_LINE;
	print_io(cmd, write, &main);
	free(write);
}
