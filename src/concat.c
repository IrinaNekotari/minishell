#include "minishell.h"

char	*ft_concat2(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	res = (char *) malloc(sizeof(char)
			* (ft_strlen(s1) + ft_strlen(s2) + 2));
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[i] = 0;
	return (res);
}

static void	super_concat(char **a, char **b)
{
	char	*bck;

	bck = ft_concat((*a), (*b));
	free((*a));
	(*a) = ft_strdup(bck);
	free(bck);
}

/**
* Transforme un char** en char*
*/
char	*flatten(char **s)
{
	int		i;
	char	*ret;

	i = 1;
	if (!s || !s[0])
		return (NULL);
	if (!s[1])
		return (ft_strdup(s[0]));
	else
	{
		ret = ft_strdup(s[0]);
		while (s[i])
		{
			super_concat(&ret, &s[i]);
			i++;
		}
	}
	return (ret);
}
