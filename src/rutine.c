/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rutine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:52:38 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/08 01:26:01 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	*rutine(void *arg)
{
	t_coder		*coder;
	t_monitor	*monitor;

	coder = (t_coder *)arg;
	monitor = malloc(sizeof(t_monitor));
	monitor->coder = coder;
	waiter(coder->shared, coder);
	coder->last_compile = coder->shared->go_t;
	pthread_create(&monitor->monitor, NULL, monitor_func, monitor);
	while (read_compiles(coder) != 0 && simulation(coder->shared) == true)
	{
		take_dongles(coder);
		actions(coder, 'c');
		releasdongles(coder);
		actions(coder, 'd');
		actions(coder, 'r');
		change_compiles(coder);
	}
	waiter_end(coder->shared, coder);
	set_simulation(coder->shared);
	pthread_join(monitor->monitor, NULL);
	wake_up_all_dongles(coder);
	free(monitor);
	return (NULL);
}
