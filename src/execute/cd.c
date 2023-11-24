/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:54:52 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/23 13:54:55 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void	ft_cd(t_cmd *cmd, t_main **main)
{
	char *oldpwd;
	//Deux variables, mais tu les utilises jamais en 
	//même temps : pas besoin des deux
	char *newpwd;
	int	i;
	//Tu n'as besoin que de i
	int	j;
	int	k;
	//CD ret est de type DIR*
	int	cd_ret;

	//Non; effectue le test AVANT d'alloc - la, tu risque d'essayer d'allocer un truc qui existe pas
	//Et utilise ft_calloc, pas malloc
	newpwd = (char *)malloc(sizeof(char) * (ft_strlen(cmd->tokens->next->str)));
	j = 0;
	//Gestion d'erreur ? Ici, tu regardes si le 3em token existe :
	//Si c'est le cas, tu renvoies l'erreur
	//cd: too many arguments
	//et tu quittes la fonction
	if (cmd->tokens->next->next->str)
		ft_printf("string not in pwd: %s\n", cmd->tokens->next->next->str);
	// ça, ça m'a l'air bon, mais ton premier test sert a rien
	//T'es dans l'execute : si t'es déjà la, cmd->tokens existe
	//forcément
	else if (cmd->tokens && !cmd->tokens->next)
		update_env(&(*main)->env, "PWD", (*main)->initpwd);
	//La condition est bonne
	//Mais tu peux rajouter un truc : écrire ../ et .. c'est la meme chose
	else if (ft_equals(cmd->tokens->next->str, ".."))
	{
		//Non; ce que tu fais la, c'est copier un pointeur
		//Donc, si tu modifies oldpwd, tu vas aussi modifier
		//initpwd ... pas bon du tout, fait un ft_strdup
		oldpwd = (*main)->initpwd;
		//Non; Tu n'as pas compris a quoi sert initpwd
		//initpwd est une sauvegarde, il ne doit
		//JAMAIS être modifié : modifie oldpwd
		i = ft_strlen(oldpwd);
		j = 0;
		//Oui
		while (oldpwd[i] != '/')
			i--;
		//Tu te compliques trop la vie :
		//Au lieu de recopier (ce qui au passage, 
		//Ne fera rien du tout avec ta boucle)
		//Mets tout simplement a 0 tous les char
		//dont l'indice est supérieur a i
		//En gros,
		/*
		while (oldpwd[i])
		{
			oldpwd[i] = 0;
			i++;
		}
		*/
		while (i > j)
		{
			(*main)->initpwd[j] = oldpwd[j];
			j++;
		}
		j++;
		(*main)->initpwd[j] = '\0';
		//Pour résumer, tu transformeras
		// /home/truc/bite
		//en /home/truc00000
		//Presque : tu envoies oldpwd, pas le initpwd
		update_env(&(*main)->env, "PWD", (*main)->initpwd);
		//Oublie pas de free oldpwd ici
		//Ajoute un sécurité par contre
		//si PWD ne vaut que "/", cd .. ne fait rien
	}	
	//La condition est bonne
	else if (cmd->tokens->next->str[0] == '/') //absolute path
	{
		//Appelles l'update APRES tes checks ... Sinon ils
		//servent a rien
		update_env(&(*main)->env, "PWD", cmd->tokens->next->str);
		//Pas la bonne fonction - t'es tombé dans le piège
		//chdir change la variable PWD du shell
		//Mais nous, on est pas de le shell, mais dans le minishell !
		//Utilises opendir pour vérifier si le dossier existe
		cd_ret = chdir(cmd->tokens->next->str);
		if (cd_ret < 0)
			cd_ret *= -1;
		if (cd_ret != 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd(": ", 2);
		}
		//et bien sur, closedir
		//opendir fonctionne comme open
		//ils renvoient un fd correspondant au dossier
		//Sauf que la, le fd est un objet, pas un int
		//donc, tu testes pas si fd == -1, mais
		//si fd == NULL
		//L'idée de balancer errno était bonne
		//Mais utilises pas putstr_fd, c'est l'antichrist
		//De plus, la sortie d'erreur n'existe pas
		//sur notre minishell :
		//utilises mes fonctions d'erreur
		//error_exec prends un int en entrée
		//(errno), et mets le message automatiquement
		//Oublie pas return !
	}
	else //relative path
	{
		//Tu te fais chier pour rien :
		//On a fait des fonctions pour ça ...
		//Regarde du coté de super_concat
		//T'init ton oldpwd a rien du tout
		//avec un ft_calloc(1, sizeof(char)
		//Ensuite, tu vas faire trois appels
		//successifs a super_concat
		//super_concat prends trois args :
		//l'adresse de ce que tu veux modifier
		//et ce que tu veux y rajouter
		//Donc, t'envoies a super_concat
		//l'addresse de oldpwd (&oldpwd);
		//et, dans l'ordre : 
		//La valeur du pwd actuel
		//(Attention ! getenv va allouer de la mémoire !
		//Tu vas avoir besoin d'une variable buffer
		//pour pouvoir free après !)
		//un slash /
		//et ton char* récupéré (donc, cmd->tokens->next->str)
		//Ensuite, comme au dessus, tu vas utiliser
		//opendir pour vérifier si le dit fichier
		//existe : S'il n'existe pas, message d'erreur
		//Sinon, tu peux appeller update_env
		oldpwd = (*main)->initpwd;
		i = ft_strlen(oldpwd);
		j = 0;
		k = 0;
		
		while (newpwd[j] == oldpwd[j])
		{
			newpwd[j] = oldpwd[j];
			j++;
		}
		while (newpwd[j])
		{
			newpwd[j] = cmd->tokens->next->str[k];
			j++;
			k++;
		}
		//Tu utilises les deux, pas besoin de les void
		//Le principe de voider un argument ?
		//si tu n'utilises pas un argument
		//le compilateur va gueuler
		//(void) un argument ne fait strictement rien
		//Par contre, ça compte comme utiliser une variable ...
		(void)cmd;
		(void)main;
	}
	//Pourquoi c'est en dehors du if ça ? lol
	newpwd[j] = '\0';
	update_env(&(*main)->env, "PWD", newpwd);
	//Il te manque un cas
	//celui du cd . qui fait rien (. et ./ sont la même chose)
	//De plus, écrire cd bite et cd ./bite c'est la même chose
	//Ajoute un check pour ça peut être ? Je suis pas sur que
	//ce soit demandé, demande a Thomas ou un autre qui carry
}
