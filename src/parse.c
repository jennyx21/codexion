/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 09:35:46 by jtruckse          #+#    #+#             */
/*   Updated: 2026/05/15 14:34:32 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	process_args(char **av)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!atoi(av[i]))
		{
			error_m2(i);
			return (0);
		}
		i++;
	}
	if ((strcmp(av[i], "fifo") != 0) && (strcmp(av[i], "edf") != 0))
		error_m2(i);
	return (1);
}
