/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjuette <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:04:17 by mjuette           #+#    #+#             */
/*   Updated: 2023/11/20 10:04:19 by mjuette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

void	ft_export(t_cmd *cmd, t_env *env)
{

	if (get_env(env, env->name) == NULL)
	{
		add_to_env(&env, env->name, env->value)
	}
	else
	{
		//unset et rajouter la ligne.
		ft_unset(cmd, env);
		add_to_env(&env, env->name, env->value)
	}
	return ;
}
