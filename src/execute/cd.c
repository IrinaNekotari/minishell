#include "minishell.h"

/*
Toutes les options possibles
cd : Renvoie a "home"
cd . : fait rien
cd .. : Renvoie au dossier supérieur, pas a traiter normalement
cd (dossier inexistant) : renvoie une erreur
cd (dossier existant) : modifie PWD dans env vers le nouveau dossier
cd (chemin inexistant) : renvoie une erreur
cd (chemin existant) : change le PWD dans env par la nouvelle valeur
cd a b -> Erreur too many arguments
*/
void	ft_cd(t_cmd *cmd, t_env *env)
{
	(void)cmd;
	(void)env;
	//TODO
}
