/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 14:58:15 by amoutik           #+#    #+#             */
/*   Updated: 2019/03/11 16:24:37 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		is_special(char c)
{
	char	*escapchar = " !@#$%^&*()-_+=,;.\"\n\t\v\f\r";
	if (ft_strchr(escapchar, c))
		return (1);
	return (0);
}

/*
 ** Remember to remove "getenv" function and replace it with local one
 ** There is also strndup function
 */

// see comment
int		handle_dollar(char **line, char **new_line, int *i)
{
	char *head;
	char *env;
	char *tmp;

	if (is_special(*((*line) + 1)))
		return (0);
	head = ++(*line);
	while (*head && !is_special(*head))
		head++;
	tmp = ft_strndup(*line, head - *line);
	if ((env = getenv(tmp)) == NULL)
		env = "";
	free(tmp);
	while (*env)
		(*new_line)[(*i)++] = *env++;
	*line = head;
	return (1);
}

char	*handle_quotes(t_line *current_line, char *new_line, t_command_list *commands)
{
	int		flag;
	int		flag_d;
	int		i;
	char	*line;
	char	*start;

	flag = 0;
	flag_d = 0;
	i = 0;

	if (current_line->old_command != NULL)
		line = ft_strjoin(current_line->old_command, current_line->command);
	else
		line = ft_strdup(current_line->command);
	start = line;
	while (*line)
	{
		if (flag_d == 0 && *line == '\'')
		{
			if((flag ^= 1) == 0)
			{
				new_line[i++] = *line++;
				push(commands, ft_strndup(new_line, i));
				i = 0;
				continue;
			}
		}
		if (flag == 0 && *line == '\\')
			ft_memmove(line, line + 1, ft_strlen(line));
		if (flag == 0 && *line == '\"')
		{
			if ((flag_d ^= 1) == 0)
			{
				new_line[i++] = *line++;
				push(commands, ft_strndup(new_line, i));
				i = 0;
				continue;
			}
		}
		if (flag_d == 0 && *line == '\\')
			line++;
		else if (flag_d && *line == '$' && handle_dollar(&line, &new_line, &i))
			continue;
		if (flag_d == 0 && flag == 0 && (*line == ' ' || *line == '\t'))
		{
			push(commands, ft_strndup(new_line, i));
			i = 0;
		}
		new_line[i++] = *line;
		line++;
	}
	free(start);
	if (flag || flag_d)
	{
		write(1, "> ", 2);
		current_line->old_command = ft_strjoin_pre_free(current_line->old_command, current_line->command, "\n");
		free_line();
		current_line = init_line();
		current_line->print_msg = 0;
		read_line(current_line);
		free_list(commands);
		return (handle_quotes(current_line, new_line, commands));
	}
	new_line[i] = '\0';
	return (new_line);
}

char	*init_quotes(t_line *line)
{
	char			*new_line;
	t_command_list	commands;

	init_list(&commands);
	new_line = ft_strnew(1000);
	ft_bzero(new_line, 1000);
	line->command = handle_quotes(line, new_line, &commands);
	print_list(&commands);
	return (line->command);
}
