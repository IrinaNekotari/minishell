#include "minishell.h"

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
void    update_env(t_env **env, char *name, char *newvalue)
{
    while ((*env))
    {
        if (ft_equals((*env)->name, name))
        {
            free((*env)->value)
            (*env)->value = ft_strdup(newvalue);
        }
        (*env) = (*env)->next;
    }
    rollback_env(env);
    return ;
}

//TODO : Retire le maillon de la chaine qui correspond a name
	//Indices : Parcourt la chaine jusqu'a tomber sur le bon name
	//Ne fait rien si t'arrives au bout
	//Pour retirer le chainon, inspire toi de ma fonction "slash_tokens"
	//Qui est dans io.c
	//Puis, rembobine la chaine avec ça
	//void	rollback_env(t_env **env);
void    del_from_env(t_env **env, char *name)
{
    t_env *element;
    while ((*env))
    {
        if (ft_equals((*env)->name, name))
        {
            element = (*env);
            free((*env)->name);
            free((*env)->value);
            if ((*env)->next)
                  (*env)->next->previous = (*env)->previous;
            if ((*env)->previous)
                  (*env)->previous->next = (*env)->next;
            free(element);
            (*env) = (*env)->next;
        }
        (*env) = (*env)->next;
    }
    rollback_env(env);
}

//TODO : Ajoute le doublet name=value dans l'env.
	//Indices : Parcours la chaine jusqu'au bout
	//C'est un ajout tout con
	//Regarde comment j'ai fait dans le generate env
	//Subtilité : si name existe déjà, mets a jour la value
	//(utilise update_env)
	//Puis, rembobine la chaine avec ça
	//void	rollback_env(t_env **env);
void    add_to_env(t_env **env, char *name, char *value)
{
    while ((*env))
    {
        if (ft_equals((*env)->name, name))
        {
          free((*env)->value)
          (*env)->value = ft_strdup(value);
          rollback_env(env);
          return ;
        }
    }
    (*env)->next = ft_calloc(1, sizeof(t_env));
    (*env)->next->previous = (*env);
    (*env) = (*env)->next;
    (*env)->name = ft_strdup(name);
    (*env)->value = ft_strdup(value);
    (*env)->next = NULL;
    rollback_env(env);
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
