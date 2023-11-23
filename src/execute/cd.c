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
void	ft_cd(t_cmd *cmd, t_main **main)
{
	//(void)cmd;
	//(void)main;
	char *oldpwd;
	int	i;
	int	j;
	int	cd_ret;
	//Tu vas devoir jouer avec les (*main) et &((*main)->env) ...
	//Ouille ouille ouille
	//Mais tu sais maintenant comment on fait, non ? :)
	//Indices : T'auras besoin de update env dans tous les cas
	//Il faudra que tu update la variable d'environnement "PWD"
	//Commence par le plus simple : l'erreur
	if (cmd->tokens->next->next->str)
		ft_printf("string not in pwd: %s\n", cmd->tokens->next->next->str);
	//Puis, par cd tout seul : Remets PWD a initpwd ...
	else if (cmd->tokens && !cmd->tokens->next)
		update_env(&(*main)->env, "PWD", (*main)->initpwd);
	//cd .. : récupère PWD dans un char, qui sera au format
	///home/truc/bite/nichons/minishell
	//Puis, tu dégages tout jusqu'a trouver un / ...
	//Donc tu auras PWD=home/truc/bite/nichons
	else if (ft_equals(cmd->tokens->next->str, ".."))
	{
		//oldpw = malloc(sizeof(char) * ft_strlen(initpwd) + 1);
		oldpwd = (*main)->initpwd;
		i = ft_strlen(oldpwd);
		j = 0;
		while (oldpwd[i] != '/')
			i--;
		while (i > j)
		{
			(*main)->initpwd[j] = oldpwd[j];
			j++;
		}
		j++;
		(*main)->initpwd[j] = '\0';
		update_env(&(*main)->env, "PWD", (*main)->initpwd);
	}	
	//simple, non ?
	//Pour les deux autres cas, regardes les fonctions
	//autorisées dans le sujet. T'en a une qui "ouvre" un
	//dossier ... Ou renvoie une erreur si ce dernier n'existe
	//pas. S'il existe ... Tu as juste a remplacer PWD par ça ...
	//Pour le chemin absolu, c'est tout con : tu remplaces juste
	else if (cmd->tokens->next->str[0] == '/') //absolute path
	{
		update_env(&(*main)->env, "PWD", cmd->tokens->next->str);
		cd_ret = chdir(cmd->tokens->next->str);
		if (cd_ret < 0)
			cd_ret *= -1;
		if (cd_ret != 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd(": ", 2);
		}
	}
	//Pour le chemin relatif, va falloir concat ...
	else //relative path
	{
		(void)cmd;
		(void)main;
	}	
}
