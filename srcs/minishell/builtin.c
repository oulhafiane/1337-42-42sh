/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/02 11:26:41 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/04/20 18:47:56 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
**	called from init_builtin,
**	it create a new element of struct t_builtin and fill it,
**	and call ft_lstadd libft function to add this element to the list
*/

static void	add_builtin(t_list **lst, char *cmd, void (*f)())
{
	t_builtin	*elem;

	elem = (t_builtin*)malloc(sizeof(t_builtin));
	elem->cmd = (char*)malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	ft_strcpy(elem->cmd, cmd);
	elem->f = f;
	ft_lstadd(lst, ft_lstnew(elem, 0));
}

/*
**	function that initiates the builtins commands
**	it adds struct t_builtin elements to the list by calling add_builtin
**	struct t_builtin have a name and a function check minishell.h
*/

void		init_builtin(t_list **lst)
{
	add_builtin(lst, "echo", &ft_echo);
	add_builtin(lst, "cd", &ft_cd);
	add_builtin(lst, "env", &ft_env);
	add_builtin(lst, "pwd", &ft_pwd);
	add_builtin(lst, "setenv", &ft_setenv);
	add_builtin(lst, "unsetenv", &ft_unsetenv);
}

/*
**	this function needed by ft_lstsearch libft function
**	that called from minishell loop function
**	to check if the command is a builtin function or not
*/

int			check_builtin(t_list *elem, void *obj)
{
	if (ft_strcmp(((t_builtin*)elem->content)->cmd, (char*)obj) == 0)
		return (1);
	else
		return (0);
}

/*
**	it frees all memory allocated in the heap created by add_builtin
**	and frees the t_list element.
*/

void		free_builtin(t_list *lst)
{
	t_list		*previous;

	while (lst)
	{
		free(((t_builtin*)lst->content)->cmd);
		free((t_builtin*)lst->content);
		previous = lst;
		lst = lst->next;
		free(previous);
	}
}

/*
** Running a builtin command
*/

void		run_builtin(t_list **env, t_list *bltin, t_command_list *command)
{
	t_redirect	*redirect;
	int			stdout_copy;
	int			stdin_copy;
	int			stderr_copy;

	redirect = handle_redirect(command);
	if (loop_dup2(redirect->dup_head, 0))
	{
		ft_printf_fd(2, "Ambiguous input redirect.\n");
		free_duped(redirect);
		return ;
	}
	stdin_copy = dup(0);
	stdout_copy = dup(1);
	stderr_copy = dup(2);
	if (loop_dup(redirect->dup_head, 0))
		((t_builtin*)bltin->content)->f(redirect->command + 1, env);
	dup2(stdin_copy, 0);
	dup2(stdout_copy, 1);
	dup2(stderr_copy, 2);
	close(stderr_copy);
	close(stdin_copy);
	close(stdout_copy);
	free_duped(redirect);
}
