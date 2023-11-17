#include "minishell.h"

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
	//Puis, rembobine la chaine avec ça
	//void	rollback_env(t_env **env);
}

char	*ft_getenv(t_env *env, char *search)
{
	char	*ret;

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

char	*generate_env(char *env, int meth)
{
	int	i;
	int	j;
	char	*a;
	char	*b;

	i = -1;
	j = -1;
	a = ft_calloc(ft_strlen(env), sizeof(char));
	b = ft_calloc(ft_strlen(env), sizeof(char));
	while (env[++i] && env[i] != '=')
		a[i] = env[i];
	i++;
	while (env[i])
		b[++j] = env[i++];
	if (meth == 1)
	{
		free(b);
		return (a);
	}
	else
	{
		free(a);
		return (b);
	}
}

t_env	*make_env(char **env)
{
	int		i;
	t_env	*ret;

	i = 1;
	ret = ft_calloc(1, sizeof(t_env));
	ret->previous = NULL;
	ret->name = generate_env(env[0], 1);
	ret->value = generate_env(env[0], 2);
	while (env[i])
	{
		ret->next = ft_calloc(1, sizeof(t_env));
		ret->next->previous = ret;
		ret = ret->next;
		ret->name = generate_env(env[i], 1);
		ret->value = generate_env(env[i], 2);
		ret->next = NULL;
		i++;
	}
	while (ret->previous)
		ret = ret->previous;
	return (ret);
}
