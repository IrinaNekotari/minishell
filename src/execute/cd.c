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

	char *buffer;
	int	i;

	DIR	*cd_ret;

	if (cmd->tokens->next->next->str)
	{
		error_print(ERROR, "cd: too many arguments", NULL);
		return ;
	}
	else if (!cmd->tokens->next)
		update_env(&((*main)->env), "PWD", (*main)->initpwd);
	else if (ft_equals(cmd->tokens->next->str, ".") || ft_equals(cmd->tokens->next->str, "./"))
		return;
	else if (ft_equals(cmd->tokens->next->str, "..") || ft_equals(cmd->tokens->next->str, "../"))
	{
		if (ft_equals(ft_getenv(((*main)->env), "PWD"), "/"))
			return ;
		oldpwd = ft_getenv(((*main)->env), "PWD");
		i = ft_strlen(oldpwd);
		while (oldpwd[i] != '/' && i > 1)
			i--;
		while (oldpwd[i])
		{
			oldpwd[i] = 0;
			i++;
		}
		update_env(&(*main)->env, "PWD", oldpwd);
		free(oldpwd);
		
	}
	else if (cmd->tokens->next->str[0] == '/') //absolute path
	{
		cd_ret = opendir(cmd->tokens->next->str);
		if (cd_ret == NULL)
			error_exec(errno);
		else
		{
			update_env(&(*main)->env, "PWD", cmd->tokens->next->str);
			closedir(cd_ret);
		}
		return ;
	}
	else //relative path
	{
		oldpwd = ft_calloc(1, sizeof(char));
		buffer = ft_getenv(((*main)->env), "PWD");
		super_concat(&oldpwd, buffer);
		super_concat(&oldpwd, "/");
		super_concat(&oldpwd, cmd->tokens->next->str);
		cd_ret = opendir(oldpwd);
		if (cd_ret == NULL)
			error_exec(errno);
		else
		{
			update_env(&(*main)->env, "PWD", oldpwd);
			closedir(cd_ret);
		}
		free(buffer);
		return ;
	}
}
