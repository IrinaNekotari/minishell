#include "minishell.h"

/**
* NB : env renvoie une erreur si elle a quoi que ce soit APRES la commande
* en dehors des flags mais comme on n'a pas a les gérer on s'en tappe
*/
void	ft_env(t_cmd *cmd, char **env)
{
	int	i = 0;
	(void)cmd;
	while (env[i])
	{
		ft_printf("%d - %s\n", i, env[i]);
		i++;
	}
}
