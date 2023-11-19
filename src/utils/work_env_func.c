#include "minishell.h"

void	update_env(t_env **env, char *name, char *newvalue)
{
	(void)env;
	(void)name;
	(void)newvalue;
	//TODO : Mets a jour la variable d'environnement
	//décrie par la clé "name"
	//parcourt la chaine jusqu'a trouver le bon name
	//Si tu trouves rien, la fonction ne fait RIEN
	//Indices ... Jette un oeil a 
	//void	super_concat(char **a, char *b);
	//dans concat.c
	//Indice 2 : T'utilises un t_env **, pas un t_env *
	//C'est comme pour les int
	// modifier un int dans une fonction, ça fait rien
	//donc, la fonction doit prendre un int *
	//C'est un peu plus chiant a écrire ici
	//Quand tu appelles un morceau de env, écris (*env)
	//genre (*env)->value
	//Puis, rembobine la chaine avec ça
	//void	rollback_env(t_env **env);
}

void	del_from_env(t_env **env, char *name)
{
	(void)env;
	(void)name;
	//TODO : Retire le maillon de la chaine qui correspond a name
	//Indices : Parcourt la chaine jusqu'a tomber sur le bon name
	//Ne fait rien si t'arrives au bout
	//Pour retirer le chainon, inspire toi de ma fonction "slash_tokens"
	//Qui est dans io.c
	//Puis, rembobine la chaine avec ça
	//void	rollback_env(t_env **env);
}

void	add_to_env(t_env **env, char *name, char *value)
{
	(void)env;
	(void)name;
	(void)value;
	//TODO : Ajoute le doublet name=value dans l'env. 
	//Indices : Parcours la chaine jusqu'au bout
	//C'est un ajout tout con
	//Regarde comment j'ai fait dans le generate env
	//Subtilité : si name existe déjà, mets a jour la value
	//(utilise update_env)
	//Puis, rembobine la chaine avec ça
	//void	rollback_env(t_env **env);
}

char	*ft_getenv(t_env *env, char *search)
{
	char	*ret;

	if (!search)
		return (NULL);
	while (env)
	{
		if (ft_equals(env->name, search))
		{
			ret = ft_strdup(env->value);
			while (env->previous)
				env = env->previous;
			return (ret);
		}
		env = env->next;
	}
	return (NULL);
}
