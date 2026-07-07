/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:53:36 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/08 01:24:19 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	create_dongles(int n, t_dongle *dongles, char *arg)
{
	int			i;
	long long	cooldown;

	i = 0;
	cooldown = (long long)atoi(arg);
	while (i < n)
	{
		dongles[i].id = i;
		dongles[i].cooldown = cooldown;
		dongles[i].taken = 0;
		dongles[i].waiting_count = 0;
		pthread_mutex_init(&dongles[i].dongle, NULL);
		pthread_cond_init(&dongles[i].d_cond, NULL);
		i++;
	}
	return ;
}

void	give_values(t_coder *coders, t_dongle *dongle,
					t_shared *shared, char **args)
{
	int	i;

	i = 0;
	while (i < shared->nb)
	{
		coders[i].id = i;
		coders[i].left_dongle = &dongle[i];
		if (i != shared->nb - 1)
			coders[i].right_dongle = &dongle[i + 1];
		else
			coders[i].right_dongle = &dongle[0];
		coders[i].time_to_burnout = (long long)atoi(args[2]);
		coders[i].time_to_compile = (long long)atoi(args[3]);
		coders[i].time_to_debug = (long long)atoi(args[4]);
		coders[i].time_to_refactor = (long long)atoi(args[5]);
		coders[i].compiles_required = (long long)atoi(args[6]);
		coders[i].shared = shared;
		i++;
	}
	return ;
}

void	create_shared(t_shared *shared)
{
	pthread_cond_init(&shared->cond, NULL);
	pthread_mutex_init(&shared->mutex, NULL);
	pthread_mutex_init(&shared->print, NULL);
	pthread_mutex_init(&shared->waiter, NULL);
	shared->simulation = true;
	shared->req_count = 0;
}

void	create_coders(t_coder *coders, t_dongle *dongle,
						t_shared *shared, char **args)
{
	int				i;
	struct timeval	tv;

	i = 0;
	gettimeofday(&tv, NULL);
	create_shared(shared);
	give_values(coders, dongle, shared, args);
	while (i < shared->nb)
	{
		pthread_create(&coders[i].coder, NULL, rutine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < shared->nb)
	{
		pthread_join(coders[i].coder, NULL);
		i++;
	}
	pthread_mutex_destroy(&shared->mutex);
	pthread_cond_destroy(&shared->cond);
	pthread_mutex_destroy(&shared->print);
	pthread_mutex_destroy(&shared->waiter);
	return ;
}
