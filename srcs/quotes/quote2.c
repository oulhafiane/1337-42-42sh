/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 16:03:34 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/04 09:51:56 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		space(char c)
{
	if (c == 'n')
		return ('\n');
	else if (c == 'a')
		return ('\a');
	else if (c == 'f')
		return ('\f');
	else if (c == 'v')
		return ('\v');
	else if (c == 'r')
		return ('\r');
	else if (c == 'b')
		return ('\b');
	else
		return (0);
}

int				check_quote_ending(char **line, int flag)
{
	char 	*ptr;
	t_line	*new_line;

	if (flag)
	{
		ptr = ft_strdup(*line);
		free_line();
		new_line = init_line();
		new_line->print_msg = 0;
		ft_printf(GET_MSG(new_line->print_msg));
		read_line(new_line);
		*line = ft_strjoin(ptr, "\n");
		ft_strdel(&ptr);
		ptr = *line;
		*line = ft_strjoin(ptr, new_line->command);
		ft_strdel(&ptr);
		ft_strdel(&new_line->command);
		new_line->command = *line;
		return (1);
	}
	return (0);
}

int				insert_token(t_token_list *list,
				t_string *str, enum token_type type)
{
	if (str->len)
	{
		push_token(list, ft_strdup(str->string), type);
		new_string(str);
	}
	return (1);
}

static int		is_special_token(char **ptr,
				t_string *str, int inquote, enum token_type *type)
{
	if (!inquote && **ptr == '&' && !(*type & SH_REDIRECTION))
		return (0);
	if (!inquote && str->len == 1 && ft_isdigit(*((*ptr) - 1)) &&
			!(*type & SH_REDIRECTION) && is_special_char(**ptr))
	{
		*type = SH_REDIRECTION;
		while (**ptr && is_special_char(**ptr))
			push(str, *(*ptr)++);
		while (**ptr && !ft_isspace(**ptr) && !is_special_char(**ptr))
			push(str, *(*ptr)++);
		if (**ptr && is_special_char(**ptr))
			return (0);
	}
	if (!inquote && ft_strchr(SH_TOKEN, **ptr))
		return (0);
	return (1);
}

static void		escaped_char(char **ptr, t_string *str)
{
	char	escaped;

	if (*((*ptr) + 1) != EOS)
	{
		if ((escaped = space(*((*ptr) + 1))))
			push(str, escaped);
		if (escaped == 0)
			push(str, *((*ptr)));
		else
			(*ptr)++;
	}
}

static int		is_quote(char c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
}

int				split_quote(t_token_list *list, char **ptr,
				t_string *str, enum token_type	type)
{
	int				inquote;
	char			quote;

	inquote = 1;
	quote = *(*ptr)++;
	while (**ptr &&
			(inquote || (!ft_isspace(**ptr) && !inquote)))
	{
		if (!is_special_token(ptr, str, inquote, &type))
			break ;
		if (**ptr == '\\')
			escaped_char(ptr, str);
		else if (!inquote && is_quote(**ptr) && ++inquote)
			quote = **ptr;
		else if (**ptr && **ptr == '$' && quote != '\'' && handle_dollar(ptr, str))
			continue;
		else if (**ptr && **ptr != quote)
			push(str, *(*ptr));
		else if (**ptr == quote)
		{
			inquote = 0;
			quote = 0;
		}
		++(*ptr);
	}
	if (!inquote)
		insert_token(list, str, type);
	return (inquote);
}