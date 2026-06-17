/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:24:43 by jtruckse          #+#    #+#             */
/*   Updated: 2026/06/17 09:52:33 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void	fifo_schedule(t_coder *coders)
{
	int		i;

	i = coders->compiles_required;
	while(i > 0)
	{
		
	}
	
}

void	*say_alive(void *arg)
{
	t_coder				*coder;
	char				*schedule;

	coder = (t_coder *)arg;
	schedule = coder->schedule;
	if (strcmp(schedule, "fifo") == 0)
		fifo_schedule(coder);
	else
		edf_schdule();

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
		coders[i].left_dongle = dongle[i];
		if (i != n - 1)
			coders[i].right_dognle = dongle[i + 1];
		else
			coders[i].right_dognle = dongle[0];
		coders[i].time_to_bournout = (long long)atoi(args[2]);
		coders[i].time_to_compile = (long long)atoi(args[3]);
		coders[i].time_to_debug = (long long)atoi(args[4]);
		coders[i].time_to_refactor = (long long)atoi(args[5]);
		coders[i].compiles_required = (long long)atoi(args[6]);
		coders[i].schedule = args[8];
		coders[i].start_time = tv;
		coders[i].coder = pthread_create(&coders[i].coder, NULL, say_alive, &coders[i]);
		i++;
	}
	pthread_join(coders[i].coder, NULL);
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
	printf("%s\n", coders[i].schedule);
	while(i < n)
	{
		printf("coder id: %d left hand dongle: %d right hand dongle %d\n", coders[i].id, coders[i].left_dongle.id, coders[i].right_dognle.id);
		printf("dongle cooldown %lld\n", dongles[i].cooldown);
		i++;
	}
	free(dongles);
	free(coders);
}
