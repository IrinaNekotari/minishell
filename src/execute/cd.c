#include "minishell.h"

/*
Toutes les options possibles
cd : Renvoie a "home" (le char* initpwd de main)
cd . : fait rien
cd .. : Renvoie au dossier supérieur, pas a traiter normalement
cd (dossier inexistant) : renvoie une erreur
cd (dossier existant) : modifie PWD dans env vers le nouveau dossier
cd (chemin inexistant) : renvoie une erreur
cd (chemin existant) : change le PWD dans env par la nouvelle valeur
cd a b -> Erreur too many arguments
*/
void	ft_cd(t_cmd *cmd, t_main **main)
{
	(void)cmd;
	(void)main;
	//Tu vas devoir jouer avec les (*main) et &((*main)->env) ...
	//Ouille ouille ouille
	//Mais tu sais maintenant comment on fait, non ? :)
	//Indices : T'auras besoin de update env dans tous les cas
	//Il faudra que tu update la variable d'environnement "PWD"
	//Commence par le plus simple : l'erreur
	//Puis, par cd tout seul : Remets PWD a initpwd ...
	//cd .. : récupère PWD dans un char, qui sera au format
	///home/truc/bite/nichons/minishell
	//Puis, tu dégages tout jusqu'a trouver un / ...
	//Donc tu auras PWD=home/truc/bite/nichons
	//simple, non ?
	//Pour les deux autres cas, regardes les fonctions
	//autorisées dans le sujet. T'en a une qui "ouvre" un
	//dossier ... Ou renvoie une erreur si ce dernier n'existe
	//pas. S'il existe ... Tu as juste a remplacer PWD par ça ...
	//Pour le chemin absolu, c'est tout con : tu remplaces juste
	//Pour le chemin relatif, va falloir concat ...
}
