/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_values.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:44:31 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/08 01:24:54 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

bool	simulation(t_shared *shared)
{
	bool	signal;

	pthread_mutex_lock(&shared->mutex);
	signal = shared->simulation;
	pthread_mutex_unlock(&shared->mutex);
	return (signal);
}

long long	last_copile(t_coder *coder)
{
	long long	last;

	pthread_mutex_lock(&coder->shared->mutex);
	last = coder->last_compile ;
	pthread_mutex_unlock(&coder->shared->mutex);
	return (last);
}

void	set_simulation(t_shared *shared)
{
	pthread_mutex_lock(&shared->mutex);
	shared->simulation = false;
	pthread_mutex_unlock(&shared->mutex);
}

int	read_compiles(t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&coder->shared->mutex);
	i = coder->compiles_required;
	pthread_mutex_unlock(&coder->shared->mutex);
	return (i);
}

void	change_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->shared->mutex);
	coder->compiles_required--;
	pthread_mutex_unlock(&coder->shared->mutex);
}
