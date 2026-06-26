/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:24:43 by jtruckse          #+#    #+#             */
/*   Updated: 2026/06/26 20:29:23 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles( t_coder *coder)
{
	int	i;

	i = 0;
	if (coder->id % 2 == 0 || coder->id == 0)
	{
		if (pthread_mutex_lock(&coder->left_dongle->dongle) == 0)
			printf("coder %d takes the left dongle\n", coder->id);
		if (pthread_mutex_lock(&coder->right_dongle->dongle) == 0)
			printf("coder %d takes the right dongle\n", coder->id);
	}
	else
	{
		if (pthread_mutex_lock(&coder->right_dongle->dongle) == 0)
			printf("coder %d takes the right dongle\n", coder->id);
		if (pthread_mutex_lock(&coder->left_dongle->dongle) == 0)
			printf("coder %d takes the left dongle\n", coder->id);
	}
}

void	releasdongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left_dongle->dongle);
	pthread_mutex_unlock(&coder->right_dongle->dongle);
}

void	compile( t_coder coder)
{
	int	i;

	i = coder.time_to_compile;
	printf("coder %d compiles\n", coder.id);
	usleep(i);
}

void	debugging(t_coder coder)
{
	int		i;

	i = coder.time_to_debug;
	printf("coder %d is debugging\n", coder.id);
	usleep(i);
}

void	refactoring(t_coder coder)
{
	int		i;

	i = coder.time_to_refactor;
	printf("coder %d is refactoring\n", coder.id);
	usleep(i);
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

void	*say_alive(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	while (coder->compiles_required != 0)
	{
		take_dongles(coder);
		compile(*coder);
		releasdongles(coder);
		debugging(*coder);
		refactoring(*coder);
		coder->compiles_required--;
	}
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

void	create_coders(int n, t_coder *coders, t_dongle *dongle, char **args)
{
	int				i;
	struct timeval	tv;

	i = 0;
	gettimeofday(&tv, NULL);
	while (i < n)
	{
		coders[i].id = i;
		coders[i].left_dongle = &dongle[i];
		if (i != n - 1)
			coders[i].right_dongle = &dongle[i + 1];
		else
			coders[i].right_dongle = &dongle[0];
		coders[i].time_to_bournout = (long long)atoi(args[2]);
		coders[i].time_to_compile = (long long)atoi(args[3]);
		coders[i].time_to_debug = (long long)atoi(args[4]);
		coders[i].time_to_refactor = (long long)atoi(args[5]);
		coders[i].compiles_required = (long long)atoi(args[6]);
		coders[i].schedule = args[8];
		coders[i].start_time = tv;
		pthread_create(&coders[i].coder, NULL, say_alive, &coders[i]);
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_join(coders[i].coder, NULL);
		i++;
	}
	return ;
}

int	main(int ac, char **av)
{
	int			n;
	int			*id;
	t_coder		*coders;
	t_dongle	*dongles;
	int			i;

	i = 0;
	if (ac == 9)
		process_args(av);
	else
		error_m1();
	n = atoi(av[1]);
	coders = malloc(sizeof(t_coder) * (n + 1));
	dongles = malloc(sizeof(t_dongle) * (n + 1));
	if (!coders || !dongles)
		return (0);
	create_dongles(n, dongles, av[7]);
	create_coders(n, coders, dongles, av);
	free(dongles);
	free(coders);
}
