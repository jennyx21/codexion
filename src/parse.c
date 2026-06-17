/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 09:35:46 by jtruckse          #+#    #+#             */
/*   Updated: 2026/06/17 06:28:09 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	check_if_valid_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	process_args(char **av)
{
	int	i;
	int	n;

	i = 1;
	while (i < 8)
	{
		if (!check_if_valid_num(av[i]))
			error_m2(i);
		if (atoi(av[i]) < 0)
			error_m3();
		i++;
	}
	if ((strcmp(av[i], "fifo") != 0) && (strcmp(av[i], "edf") != 0))
		error_m2(i);
	return (1);
}
