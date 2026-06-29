/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:24:43 by jtruckse          #+#    #+#             */
/*   Updated: 2026/06/29 22:47:37 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	take_dongles( t_coder *coder)
{
	int	i;

	i = 0;
	if (coder->id % 2 == 0 || coder->id == 0)
	{
		if (pthread_mutex_lock(&coder->left_dongle->dongle) == 0)
			printf("%lld coder %d takes the left dongle\n",
				(get_time() - coder->shared->start_time), coder->id);
		if (pthread_mutex_lock(&coder->right_dongle->dongle) == 0)
			printf("%lld coder %d takes the right dongle\n",
				(get_time() - coder->shared->start_time), coder->id);
	}
	else
	{
		if (pthread_mutex_lock(&coder->right_dongle->dongle) == 0)
			printf("%lld coder %d takes the right dongle\n",
				(get_time() - coder->shared->start_time), coder->id);
		if (pthread_mutex_lock(&coder->left_dongle->dongle) == 0)
			printf("%lld coder %d takes the left dongle\n",
				(get_time() - coder->shared->start_time), coder->id);
	}
}

void	releasdongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left_dongle->dongle);
	usleep(coder->left_dongle->cooldown * 1000);
	pthread_mutex_unlock(&coder->right_dongle->dongle);
	usleep(coder->right_dongle->cooldown * 1000);

}
void	*monitor_func(void *arg)
{
	t_monitor	*monitor;
	t_coder		*coder;
	long long	start;

	monitor = (t_monitor *)arg;
	coder = monitor->coder;
	while (coder->shared-> simulation == true)
	{
		if (get_time() - coder->last_compile > coder->time_to_burnout)
		{
			printf("%lld coder %d burnedout\n",
				get_time() - coder->shared->start_time, coder->id);
			coder->shared->simulation = false;
			return NULL;
		}
		usleep(1000);
	}
	return (NULL);
}
void	actions( t_coder *coder, char c)
{
	int	i;

	if (c == 'c' && coder->shared->simulation == true)
	{
		i = coder->time_to_compile;
		printf("%lld coder %d compiles\n",
			(get_time() - coder->shared->start_time), coder->id);
		usleep(i * 1000);
		coder->last_compile = get_time();
	}
	else if (c == 'd' && coder->shared->simulation == true)
	{
		i = coder->time_to_debug;
		printf("%lld coder %d is debugging\n",
			(get_time() - coder->shared->start_time), coder->id);
		usleep(i * 1000);
	}
	else if (c == 'r' && coder->shared->simulation == true)
	{
		i = coder->time_to_refactor;
		printf("%lld coder %d is refactoring\n",
			(get_time() - coder->shared->start_time), coder->id);
		usleep(i * 1000);
	}
	return ;
}

void	fifo_schedule(t_coder *coders)
{
	int		i;

	i = coders->compiles_required;
	while (i > 0)
	{
		printf("%d\n", i);
		i--;
	}
}

void waiter(t_shared *shared)
{
	pthread_mutex_lock(&shared->mutex);
	shared->arived++;
	if (shared->arived == shared->nb)
	{
		shared->start_time = get_time();
		pthread_cond_broadcast(&shared->cond);
	}
	else
		while (shared->arived < shared->nb)
			pthread_cond_wait(&shared->cond, &shared->mutex);
	pthread_mutex_unlock(&shared->mutex);
	return ;
}

void	*say_alive(void *arg)
{
	t_coder		*coder;
	t_monitor	*monitor;

	coder = (t_coder *)arg;
	monitor = malloc(sizeof(t_monitor));
	monitor->coder = coder;
	waiter(coder->shared);
	coder->last_compile = coder->shared->start_time;
	pthread_create(&monitor->monitor, NULL, monitor_func, monitor);
	while (coder->compiles_required != 0 && coder->shared->simulation == true)
	{
		take_dongles(coder);
		actions(coder, 'c');
		releasdongles(coder);
		actions(coder, 'd');
		actions(coder, 'r');
		coder->compiles_required--;
	}
	coder->shared->simulation = false;
	pthread_join(monitor->monitor, NULL);
	return NULL;
}

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
		pthread_mutex_init(&dongles[i].dongle, NULL);
		i++;
	}
	return ;
}

void	give_values(t_coder *coders, t_dongle *dongle, t_shared *shared, char **args, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		coders[i].id = i;
		coders[i].left_dongle = &dongle[i];
		if (i != n - 1)
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

void	create_coders(int n, t_coder *coders, t_dongle *dongle, t_shared *shared, char **args)
{
	int				i;
	struct timeval	tv;

	i = 0;
	pthread_cond_init(&shared->cond, NULL);
	pthread_mutex_init(&shared->mutex, NULL);
	gettimeofday(&tv, NULL);
	give_values(coders, dongle, shared, args, n);
	shared->simulation = true;
	while (i < n)
	{
		pthread_create(&coders[i].coder, NULL, say_alive, &coders[i]);
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_join(coders[i].coder, NULL);
		i++;
	}
	pthread_mutex_destroy(&shared->mutex);
	pthread_cond_destroy(&shared->cond);
	return ;
}

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
	create_coders(n, coders, dongles, shared, av);
	free(dongles);
	free(coders);
}
