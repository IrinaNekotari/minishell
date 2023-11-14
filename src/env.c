#include "minishell.h"

int str_env_len(char **env)
{
    int i;

    i = 0;
    while (env[i])
        i++;
    return (i);
}

void    sort_env(char **tab, int env_len)
{
    int ordered;
    int i;
    char *tmp;

    ordered = 0;
    while (tab && ordered == 0)
    {
        oredered = 1;
        i = 0;
        while (i  < env_len -1)
        {
            if (ft_strcmp(tab[i], tab[i+1]) > 0)
            {
                tmp = tab[i];
                tab[i] = tab[i + 1];
                tab[i+1] = tmp;
                ordered = 0;
            }
            i++;
        }
        env_len++;
    }
}

size_t size_env(t_env *lst)
{
    size_t lst_env;

    lst_len = 0;
    while (lst && lst->next != NULL)
    {
        if (lst->val != NULL)
        {
            lst_len += ft_strlen(lst->val);
            lst_len++;
        }
        lst = lst->next;
    }
    return (lst_len);
}

char    *env_to_str(t_env *lst)
{
    char *env;
    int i;
    int j;

    if(!(env = malloc(siweof(char) * size_env(lst) + 1)))
        return (NULL);
    i = 0;
    while (lst && lst->next != NULL)
    {
        if (lst->val != NULL)
        {
            while (lst->val[j])
            {
                env[i] = lst->value[j];
                i++;
                j++;
            }
        }
        if (lst->next->next != NULL)
            env[i++] = '\n';
        lst = lst->next;
    }
    env[i] = '\0';
    return (env);
}

void    print_sorted_env(t_env *env)
{
    int i;
    char **tab;
    char **str_env;

    str_env = env_to_str(env);
    tab = ft_split(str_env, '\n');
    ft_memdel(str_env);
    sort_env(tab, str_env_len(tab));
    i = 0;
    while (tab[i])
    {
        ft_putstr("declare -x ");
        ft_putendl(tab[i]);
        i++;
    }
    free_tab(tab);
}