#include "minishell.h"

void	ft_pwd(t_cmd *cmd, char **env)
{
	char	*dir;

	(void)env;
	dir = (char *)ft_calloc(100, sizeof(char));
	getcwd(dir, 100);
	print_io(cmd, dir);
	free(dir);
}
