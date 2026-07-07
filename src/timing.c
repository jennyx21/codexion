/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:41:39 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/07 22:41:42 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000));
}


long long	last_copile(t_coder *coder)
{
    long long last;

    pthread_mutex_lock(&coder->shared->mutex);
    last = coder->last_compile ;
    pthread_mutex_unlock(&coder->shared->mutex);
    return (last);
}
