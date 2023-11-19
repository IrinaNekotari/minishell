#include "minishell.h"

/*
Instructions : La commande la plus dure a traiter
echo seul saute une ligne
echo $VAR : affiche la variable "var" de env
si VAR n'existe pas, n'affiche rien
Subtilité : si VAR n'existe pas et que c'est le seul argument lancé a
echo, il saute une ligne !
Si VAR est entre '', VAR n'est pas interprété
$? renvoie le dernier code d'erreur généré dans le cas général
*/
void	ft_echo(t_cmd *cmd, t_env *main)
{
	(void)cmd;
	(void)main;
	//TODO
}
