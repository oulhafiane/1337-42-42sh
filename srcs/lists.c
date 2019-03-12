/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 14:57:32 by amoutik           #+#    #+#             */
/*   Updated: 2019/03/12 15:06:22 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	init_list(t_command_list *listptr)
{
	listptr->head = NULL;
	listptr->tail = NULL;
	listptr->node_count = 0;
}

void	push(t_command_list *listptr, char *command)
{
	t_command *new_node_ptr;

	new_node_ptr = (t_command *)ft_memalloc(sizeof(t_command));
	if (new_node_ptr == NULL)
		return ;
	new_node_ptr->argv = command;
	new_node_ptr->next = NULL;
	if (listptr->node_count == 0)
	{
		listptr->head = new_node_ptr;
		listptr->tail = new_node_ptr;
	}
	else
	{
		listptr->tail->next = new_node_ptr;
		listptr->tail = new_node_ptr;
	}
	listptr->node_count++;
}

void	free_list(t_command_list *ptr)
{
	t_command	*head;
	t_command	*tmp;

	head = ptr->head;
	while (head)
	{
		tmp = head->next;
		free(head->argv);
		free(head);
		head = tmp;
	}
	init_list(ptr);
}

void		print_list(t_command_list *ptr)
{
	t_command *head;

	head = ptr->head;
	while (head)
	{
		ft_printf("%s", head->argv);
		if (head->next)
			ft_printf("\n");
		head = head->next;
	}
}

char		**list_to_chars(t_command_list *ptr)
{
	char		**cmds;
	t_command	*node;
	int			i;

	cmds = (char**)ft_memalloc(sizeof(char*) * (ptr->node_count + 1));
	node = ptr->head;
	i = 0;
	while (node)
	{
		cmds[i++] = ft_trim(node->argv);
		node = node->next;
	}
	cmds[i] = NULL;
	return (cmds);
}
