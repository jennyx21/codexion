/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:39:53 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/07 22:57:02 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../codexion.h"

void	print_action_m(pthread_mutex_t *print, char c, t_coder *coder)
{
	pthread_mutex_lock(print);
	if (c == 'c')
	{
		printf("%lld coder %d compiles\n",
			(get_time() - coder->shared->go_t), coder->id);
	}
	else if (c == 'd')
	{
		printf("%lld coder %d is debugging\n",
			(get_time() - coder->shared->go_t), coder->id);
	}
	else if (c == 'r')
	{
		printf("%lld coder %d is refactoring\n",
			(get_time() - coder->shared->go_t), coder->id);
	}
	pthread_mutex_unlock(print);
}

void	make_action(long long i, t_coder *coder)
{
	long long	start;

	start = get_time();
	while (get_time() - start < i)
	{
		if (!simulation(coder->shared))
			break ;
		usleep(1000);
	}
}

void	actions( t_coder *coder, char c)
{
	long long	i;

	if (c == 'c' && simulation(coder->shared) == true)
	{
		i = coder->time_to_compile;
		pthread_mutex_lock(&coder->shared->mutex);
		coder->last_compile = get_time();
		pthread_mutex_unlock(&coder->shared->mutex);
		print_action_m(&coder->shared->print, c, coder);
		make_action(i, coder);
	}
	else if (c == 'd' && simulation(coder->shared) == true)
	{
		i = coder->time_to_debug;
		print_action_m(&coder->shared->print, c, coder);
		make_action(i, coder);
	}
	else if (c == 'r' && simulation(coder->shared) == true)
	{
		i = coder->time_to_refactor;
		print_action_m(&coder->shared->print, c, coder);
		make_action(i, coder);
	}
	return ;
}