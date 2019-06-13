/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atof.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 10:53:43 by chorange          #+#    #+#             */
/*   Updated: 2019/06/13 20:43:17 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int		strlen_while_dig(char *str)
{
	int len;

	len = 0;
	while (str)
	{
		if (!ft_isdigit(*str))
			break ;
		str++;
		len++;
	}
	return (len);
}

double			ft_atof(char *str)
{
	double	ret;
	double	mod;
	int		neg;

	while ((*str == '\n') || (*str == '\t') || (*str == '\v') ||
			(*str == ' ') || (*str == '\f') || (*str == '\r'))
		str++;
	neg = (*str == '-' ? 1 : 0);
	ret = 0.0;
	ret = (double)ft_atoi(str);
	if (!ft_strchr(str, '.'))
		return (ret);
	str = ft_strchr(str, '.') + 1;
	if (ft_isdigit(*str))
	{
		mod = (double)ft_atoi(str);
		mod /= pow(10, strlen_while_dig(str));
		ret += (neg ? -mod : mod);
	}
	return (ret);
}

char			*ft_ftoa(double n)
{
	char *tmp;
	char *ret;
	int i;
	int f;

	ret = (char *)malloc(64);//////////////////////////
	i = (int)n;
	f = (int)((n - (double)((int)n)) * 1000.0) * (n < 0.0 ? -1.0 : 1.0);

	tmp = ft_itoa(i);
	if (!ft_strchr(tmp, '-') && n < 0.0)
		ft_strcpy(tmp, "-0");
	ft_strcpy(ret, tmp);
	free(tmp);
	

	tmp = ft_itoa(f);
	ft_strcat(ret, ".");
	ft_strcat(ret, tmp);
	free(tmp);
	
	return ret;
}
/*
int main()
{
	char * ptr = ft_ftoa(-0.7);
	puts(ptr);
	free (ptr);
	return 0;
}*/