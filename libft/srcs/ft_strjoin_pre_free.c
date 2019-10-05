/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_pre_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoulhafi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 08:42:52 by zoulhafi          #+#    #+#             */
/*   Updated: 2019/03/11 13:23:48 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_pre_free(char *s1, char *del, char const *s2)
{
	char	*tmp;

	tmp = s1;
	if (s1 == NULL && del == NULL)
		s1 = ft_strdup(s2);
	else if (s1 == NULL)
		s1 = ft_strjoin(del, s2);
	else if (del == NULL)
		s1 = ft_strjoin(s1, s2);
	else
		s1 = ft_strjoin_pre(s1, del, s2);
	free(tmp);
	return (s1);
}
