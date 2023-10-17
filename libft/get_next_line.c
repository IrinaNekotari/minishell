/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmascrie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 10:03:26 by nmascrie          #+#    #+#             */
/*   Updated: 2023/05/02 10:03:28 by nmascrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	read_and_stash(int fd, t_list2 **stash)
{
	char	*buf;
	int		lu;

	lu = 1;
	while (!found_newline(*stash) && lu != 0)
	{
		buf = malloc(sizeof(char *) * (BUFFER_SIZE + 1));
		if (buf == NULL)
			return ;
		lu = (int)read(fd, buf, BUFFER_SIZE);
		if ((*stash == NULL && lu == 0) || lu == -1)
		{
			free(buf);
			return ;
		}
		buf[lu] = '\0';
		add_to_stash(stash, buf, lu);
		free(buf);
	}
}

void	add_to_stash(t_list2 **stash, char *buf, int lu)
{
	t_list2	*new_node;
	t_list2	*last;
	int		i;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	new_node->next = NULL;
	new_node->content = malloc(sizeof(char) * (lu + 1));
	if (new_node->content == NULL)
		return ;
	i = 0;
	while (buf[i] && i < lu)
	{
		new_node->content[i] = buf[i];
		i++;
	}
	new_node->content[i] = '\0';
	if (*stash == NULL)
	{
		*stash = new_node;
		return ;
	}
	last = ft_lst_get_last(*stash);
	last->next = new_node;
}

void	extract_line(t_list2 *stash, char **line)
{
	int	i;
	int	j;

	if (stash == NULL)
		return ;
	generate_line(line, stash);
	if (*line == NULL)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				(*line)[j++] = stash->content[i];
				break ;
			}
			(*line)[j++] = stash->content[i++];
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

void	clean_stash(t_list2 **stash)
{
	t_list2	*last;
	t_list2	*clean_node;
	int		i;
	int		j;

	clean_node = malloc(sizeof(t_list));
	if (stash == NULL || clean_node == NULL)
		return ;
	clean_node->next = NULL;
	last = ft_lst_get_last(*stash);
	i = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content && last->content[i] == '\n')
		i++;
	clean_node->content = malloc(sizeof(char)
			* ((ft_strlen(last->content) - i) + 1));
	if (clean_node->content == NULL)
		return ;
	j = 0;
	while (last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(*stash);
	*stash = clean_node;
}

char	*get_next_line(int fd)
{
	static t_list2	*stash[FD_LIMIT];
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) < 0 || fd > FD_LIMIT)
		return (NULL);
	line = NULL;
	read_and_stash(fd, &stash[fd]);
	if (stash[fd] == NULL)
		return (NULL);
	extract_line(stash[fd], &line);
	clean_stash(&stash[fd]);
	if (line[0] == '\0')
	{
		free_stash(stash[fd]);
		stash[fd] = NULL;
		free(line);
		return (NULL);
	}
	return (line);
}
