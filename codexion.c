/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:24:43 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/07 20:24:40 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

bool	simulation(t_shared *shared)
{
	bool	signal;

	pthread_mutex_lock(&shared->mutex);
	signal = shared->simulation;
	pthread_mutex_unlock(&shared->mutex);
	return (signal);
}

t_request	*make_req(t_dongle *dong, t_coder *coder)
{
	t_request	*req;

	req = malloc(sizeof(t_request));
	if (!req)
		return (0);
	pthread_mutex_lock(&coder->shared->mutex);
	coder->shared->req_count++;
	req->id = coder->shared->req_count;
	req->deadline = coder->last_compile + coder->time_to_burnout;
	pthread_mutex_unlock(&coder->shared->mutex);
	if (strcmp(coder->shared->schedule, "fifo") == 0)
		req->priority = req->id;
	else
		req->priority = req->deadline;
	dong->waiting[dong->waiting_count] = req;
	dong->waiting_count++;
	return (req);
}

t_request	*best_req(t_dongle *dong)
{
	int			i;
	t_request	*result;

	if (dong->waiting_count == 0)
		return (0);
	result = dong->waiting[0];
	i = 1;
	while (i < dong-> waiting_count)
	{
		if (dong->waiting[i]->priority < result->priority)
			result = dong->waiting[i];
		i++;
	}
	return (result);
}

int	take_a_dongle(t_dongle *dongle, t_coder *coder)
{
	t_request	*req;

	pthread_mutex_lock(&dongle->dongle);
	req = make_req(dongle, coder);
	while ((dongle->taken == 1 || req != best_req(dongle))
		&& simulation(coder->shared))
		pthread_cond_wait(&dongle->d_cond, &dongle->dongle);
	if (dongle->waiting_count == 2 && dongle->waiting[0] == req)
		dongle->waiting[0] = dongle->waiting[1];
	dongle->waiting_count--;
	dongle->taken = 1;
	if (!simulation(coder->shared))
	{
		if (dongle->waiting_count > 0)
			dongle->waiting_count--;
		free(req);
		pthread_mutex_unlock(&dongle->dongle);
		return (0);
	}
	free(req);
	pthread_mutex_unlock(&dongle->dongle);
	return (1);
}
void print_d_m(int id, long long time, char c, pthread_mutex_t *print)
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
long long	last_copile(t_coder *coder)
{
	long long last;

	pthread_mutex_lock(&coder->shared->mutex);
	last = coder->last_compile ;
	pthread_mutex_unlock(&coder->shared->mutex);
	return (last);
}

void set_simulation(t_shared *shared)
{
	pthread_mutex_lock(&shared->mutex);
	shared->simulation = false;
	pthread_mutex_unlock(&shared->mutex);
}

int read_compiles(t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&coder->shared->mutex);
	i = coder->compiles_required;
	pthread_mutex_unlock(&coder->shared->mutex);
	return (i);
}

void	*monitor_func(void *arg)
{
	t_monitor		*monitor;
	t_coder			*coder;
	pthread_mutex_t	*print;

	monitor = (t_monitor *)arg;
	coder = monitor->coder;
	print = &coder->shared->print;
	while (simulation(coder->shared) == true)
	{
		if ((get_time() - last_copile(coder) > coder->time_to_burnout) && read_compiles(coder) != 0)
		{
			pthread_mutex_lock(print);
			printf("%lld coder %d burnedout\n",
				get_time() - coder->shared->go_t, coder->id);
			pthread_mutex_unlock(print);
			set_simulation(coder->shared);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

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

void	actions( t_coder *coder, char c)
{
	int	i;

	if (c == 'c' && simulation(coder->shared) == true)
	{
		i = coder->time_to_compile;
		pthread_mutex_lock(&coder->shared->mutex);
		coder->last_compile = get_time();
		pthread_mutex_unlock(&coder->shared->mutex);
		print_action_m(&coder->shared->print, c, coder);
		usleep(i * 1000);
	}
	else if (c == 'd' && simulation(coder->shared) == true)
	{
		i = coder->time_to_debug;
		print_action_m(&coder->shared->print, c, coder);
		usleep(i * 1000);
	}
	else if (c == 'r' && simulation(coder->shared) == true)
	{
		i = coder->time_to_refactor;
		print_action_m(&coder->shared->print, c, coder);
		usleep(i * 1000);
	}
	return ;
}

void waiter(t_shared *shared, t_coder *coder)
{
	pthread_mutex_lock(&shared->waiter);
	shared->arived++;
	if (shared->arived == shared->nb)
	{
		shared->go_t = get_time();
		pthread_cond_broadcast(&shared->cond);
	}
	else
		while (shared->arived < shared->nb)
			pthread_cond_wait(&shared->cond, &shared->waiter);
	pthread_mutex_unlock(&shared->waiter);
	return ;
}

void wake_up_all_Dongles(t_coder *coder)
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

void waiter_end(t_shared *shared, t_coder *coder)
{
	pthread_mutex_lock(&shared->waiter);
	shared->arived--;
	if (shared->arived == 0)
		pthread_cond_broadcast(&shared->cond);
	else
		while (shared->arived > 0)
			pthread_cond_wait(&shared->cond, &shared->waiter);
	pthread_mutex_unlock(&shared->waiter);
	return ;
}
void change_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->shared->mutex);
	coder->compiles_required--;
	pthread_mutex_unlock(&coder->shared->mutex);
}
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
	wake_up_all_Dongles(coder);
	free(monitor);
	return (NULL);
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
		dongles[i].taken = 0;
		dongles[i].waiting_count = 0;
		pthread_mutex_init(&dongles[i].dongle, NULL);
		pthread_cond_init(&dongles[i].d_cond, NULL);
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
void create_shared(t_shared *shared)
{
	pthread_cond_init(&shared->cond, NULL);
	pthread_mutex_init(&shared->mutex, NULL);
	pthread_mutex_init(&shared->print, NULL);
	pthread_mutex_init(&shared->waiter, NULL);
	shared->simulation = true;
	shared->req_count = 0;
}

void	create_coders(int n, t_coder *coders, t_dongle *dongle, t_shared *shared, char **args)
{
	int				i;
	struct timeval	tv;

	i = 0;
	gettimeofday(&tv, NULL);
	create_shared(shared);
	give_values(coders, dongle, shared, args, n);
	while (i < n)
	{
		pthread_create(&coders[i].coder, NULL, rutine, &coders[i]);
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
	pthread_mutex_destroy(&shared->print);
	pthread_mutex_destroy(&shared->waiter);
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
	free(shared);
}
