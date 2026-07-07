/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:45:59 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/08 01:23:03 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	print_d_m(int id, long long time, char c, pthread_mutex_t *print)
{
	if (c == 'l')
	{
		pthread_mutex_lock(print);
		printf("%lld coder %d takes the left dongle\n", time, id);
		pthread_mutex_unlock(print);
	}
	else if (c == 'r')
	{
		pthread_mutex_lock(print);
		printf("%lld coder %d takes the right dongle\n", time, id);
		pthread_mutex_unlock(print);
	}
}

void	take_dongles( t_coder *coder)
{
	int				i;
	pthread_mutex_t	*p;

	p = &coder->shared->print;
	i = 0;
	if ((coder->id % 2 == 0 || coder->id == 0) && simulation(coder->shared))
	{
		if (take_a_dongle(coder->left_dongle, coder) != 0)
			print_d_m(coder->id, (get_time() - coder->shared->go_t), 'l', p);
		if (take_a_dongle(coder->right_dongle, coder) != 0)
			print_d_m(coder->id, (get_time() - coder->shared->go_t), 'r', p);
	}
	else
	{
		if (take_a_dongle(coder->right_dongle, coder) != 0)
			print_d_m(coder->id, (get_time() - coder->shared->go_t), 'r', p);
		if (take_a_dongle(coder->left_dongle, coder) != 0)
			print_d_m(coder->id, (get_time() - coder->shared->go_t), 'l', p);
	}
}

void	releasdongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->dongle);
	coder->left_dongle->taken = 0;
	pthread_cond_broadcast(&coder->left_dongle->d_cond);
	usleep(coder->left_dongle->cooldown * 1000);
	pthread_mutex_unlock(&coder->left_dongle->dongle);
	pthread_mutex_lock(&coder->right_dongle->dongle);
	coder->right_dongle->taken = 0;
	pthread_cond_broadcast(&coder->right_dongle->d_cond);
	usleep(coder->right_dongle->cooldown * 1000);
	pthread_mutex_unlock(&coder->right_dongle->dongle);
}

void	wake_up_all_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->dongle);
	coder->left_dongle->taken = 0;
	pthread_cond_broadcast(&coder->left_dongle->d_cond);
	pthread_mutex_unlock(&coder->left_dongle->dongle);
	pthread_mutex_lock(&coder->right_dongle->dongle);
	coder->right_dongle->taken = 0;
	pthread_cond_broadcast(&coder->right_dongle->d_cond);
	pthread_mutex_unlock(&coder->right_dongle->dongle);
}
