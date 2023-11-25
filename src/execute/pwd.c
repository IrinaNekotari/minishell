#include "minishell.h"

void	ft_pwd(t_cmd *cmd, t_main *main)
{
	char	*dir;

	dir = ft_getenv(main->env, "PWD");
	print_io(cmd, dir, main);
	free(dir);
}
