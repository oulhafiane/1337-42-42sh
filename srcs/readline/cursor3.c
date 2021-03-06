/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 20:27:51 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/09/21 06:31:39 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		go_down(t_line *line)
{
	int		index;

	if (get_current_rows(line) <= 0)
		return ;
	index = 0;
	while (line->index < line->top &&
			line->command[line->index + 1] != '\n' && index++ < line->col)
		go_right(line);
	if (index <= line->col)
	{
		if (line->new_lines && line->new_lines->next)
			index += *((int*)line->new_lines->next->content);
		else if (line->new_lines == NULL && line->head_newlines)
			index += *((int*)line->head_newlines->content);
	}
	if (index <= line->col)
	{
		go_right(line);
		while (line->index < line->top &&
				index++ < line->col && line->command[line->index + 1] != '\n')
			go_right(line);
	}
}

void		go_up(t_line *line)
{
	int		index;

	if (line->index == line->current_index && line->index +
			(int)ft_strlen(GET_MSG(line->print_msg)) < line->col - 1)
		return ;
	index = 0;
	while (line->index >= 0 &&
			line->command[line->index] != '\n' && index++ < line->col)
		go_left(line);
	if (index < line->col)
	{
		if (line->new_lines)
			index += *((int*)line->new_lines->content);
		go_left(line);
	}
	if (index < line->col)
	{
		while (line->index >= 0 &&
				index++ < line->col && line->command[line->index] != '\n')
			go_left(line);
	}
}

void		next_word(t_line *line, int direction)
{
	int		ws;
	char	*str;

	str = line->command;
	ws = !ft_isalnum(line->command[line->index + 1]);
	if (direction == GO_RIGHT)
	{
		while (!ws && line->index < line->top &&
				ft_isalnum(str[line->index + 1]))
			go_right(line);
		while (line->index < line->top && !ft_isalnum(str[line->index + 1]))
			go_right(line);
	}
	else if (direction == GO_LEFT)
	{
		if (!ws && line->index >= 0 && ft_isalnum(str[line->index + 1]))
		{
			go_left(line);
			ws = 1;
		}
		while (ws && line->index >= 0 && !ft_isalnum(str[line->index + 1]))
			go_left(line);
		while (line->index >= 0 && ft_isalnum(str[line->index]))
			go_left(line);
	}
}

void		print_line(t_line *line, char *tmp, char buf)
{
	int		i;

	tputs(tgetstr("sc", NULL), 1, ft_putchar);
	tputs(tgetstr("cd", NULL), 1, ft_putchar);
	i = -1;
	line->count_down = 0;
	line->current_row = get_current_row(tgetnum("li"));
	if (buf != 0 && buf != -1)
		print_newchar(line, buf);
	while (tmp[++i] != '\0')
		print_newchar(line, tmp[i]);
}

void		update_line(t_line *line, char *tmp, char buf)
{
	int		index;
	int		current_index;
	int		current_rows;
	int		total_tabs;

	current_rows = get_current_rows(line);
	index = line->index;
	current_index = line->current_index;
	total_tabs = line->total_tabs;
	line->top = line->index;
	print_line(line, tmp, buf);
	if (buf != -1) // had -1 mabkitch 3a9al lach tatsla7
	{
		tputs(tgetstr("rc", NULL), 1, ft_putchar);
		line->index = index;
		line->current_index = current_index;
		line->total_tabs = total_tabs;
	}
	if (buf != 0)
		decision_top_down_left(line, current_rows);
}
