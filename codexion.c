/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:24:43 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/08 01:18:17 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	int			n;
	t_coder		*coders;
	t_dongle	*dongles;
	t_shared	*shared;
	int			i;

	i = 0;
	if (ac == 9)
		process_args(av);
	else
		error_m1();
	n = atoi(av[1]);
	coders = malloc(sizeof(t_coder) * (n + 1));
	dongles = malloc(sizeof(t_dongle) * (n + 1));
	shared = malloc(sizeof (t_shared));
	if (!coders || !dongles || !shared)
		return (0);
	shared->nb = atoi(av[1]);
	shared->schedule = av[8];
	shared->arived = 0;
	create_dongles(n, dongles, av[7]);
	create_coders(coders, dongles, shared, av);
	free(dongles);
	free(coders);
	free(shared);
}
