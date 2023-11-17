#include "minishell.h"

void	ft_pwd(t_cmd *cmd, t_env *env)
{
	char	*dir;

	dir = ft_getenv(env, "PWD");
	print_io(cmd, dir);
	free(dir);
}
