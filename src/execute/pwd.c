#include "minishell.h"

static void	print_to_sortie(t_cmd *cmd, char *meth, char *pwd)
{
	int		fd;
	t_cmd	*bck;

	bck = cmd;
	while (!ft_equals(bck->tokens->str, meth))
		bck->tokens = bck->tokens->next;
	bck->tokens = bck->tokens->next;
	if (ft_equals(meth, ">"))
		fd = open(bck->tokens->str, O_RDWR | O_CREAT);
	else
		fd = open(bck->tokens->str, O_RDWR | O_APPEND | O_CREAT);
	ft_putstr_fd(pwd, fd);
	close(fd);
}

static void	force_your_way(t_cmd *c, char *pwd)
{
	char	*answ;

	while (1)
	{
		answ = readline("> ");
		if (!answ)
		{
			ft_printf("[ERROR] : Exited prompt through end-of-file\n");
			return ;
		}
		else if (ft_equals(c->tokens->str, answ))
		{
			ft_printf("%s\n", pwd);
			return ;
		}
		free(answ);
	}
}

static void	print_from_fd(t_cmd *cmd, char * meth, char *pwd)
{
	int		fd;
	t_cmd	*bck;

	bck = cmd;
	while (!ft_equals(bck->tokens->str, ">"))
		bck->tokens = bck->tokens->next;
	bck->tokens = bck->tokens->next;
	fd = open(bck->tokens->str, O_RDONLY);
	if (fd == -1)
	{
		if (ft_equals(meth, "<"))
			ft_printf("[ERROR] : No such file or directory\n");
		else
			force_your_way(bck, pwd);
	}
	else
		ft_printf("%s\n", pwd);
	close(fd);
}

void	ft_pwd(t_cmd *cmd, char **env)
{
	char	*dir;

	(void)env;
	dir = (char *)ft_calloc(100, sizeof(char));
	getcwd(dir, 100);
	if (chain_as_equals(cmd, ">"))
		print_to_sortie(cmd, ">", dir);
	else if (chain_as_equals(cmd, ">>"))
		print_to_sortie(cmd, ">>", dir);
	else if (chain_as_equals(cmd, "<"))
		print_from_fd(cmd, "<", dir);
	else if (chain_as_equals(cmd, "<<"))
		print_from_fd(cmd, "<<", dir);
	else
		ft_printf("%s\n", dir);
	free(dir);
}
