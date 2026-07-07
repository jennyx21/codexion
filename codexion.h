/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:25:44 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/08 01:25:56 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_request
{
	long long		priority;
	int				id;
	long long		deadline;
}	t_request;

typedef struct s_shared
{
	pthread_mutex_t		mutex;
	pthread_mutex_t		print;
	pthread_mutex_t		waiter;
	pthread_cond_t		cond;
	int					nb;
	char				*schedule;
	int					arived;
	long long			go_t;
	int					req_count;
	bool				simulation;
}	t_shared;

typedef struct s_dongle
{
	int					id;
	pthread_mutex_t		dongle;
	pthread_cond_t		d_cond;
	long long			cooldown;
	int					taken;
	t_request			*waiting[2];
	int					waiting_count;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				priority_id;
	pthread_t		coder;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_shared		*shared;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	long long		last_compile;
	int				compiles_required;
}	t_coder;

typedef struct s_monitor
{
	pthread_t		monitor;
	t_coder			*coder;
	pthread_cond_t	m_cond;
	int				coders_finished;
}	t_monitor;

int			process_args(char **av);
int			ft_isdigit(int c);
int			check_if_valid_num(char *str);

void		error_m1(void);
void		error_m2(int i);
void		error_m3(void);
void		error_m4(int i);

long long	get_time(void);
bool		simulation(t_shared *shared);

void		print_action_m(pthread_mutex_t *print, char c, t_coder *coder);
void		make_action(long long i, t_coder *coder);
void		actions(t_coder *coder, char c);

void		print_d_m(int id, long long time, char c, pthread_mutex_t *print);
void		take_dongles(t_coder *coder);
void		releasdongles(t_coder *coder);
void		wake_up_all_dongles(t_coder *coder);

void		waiter(t_shared *shared, t_coder *coder);
void		waiter_end(t_shared *shared, t_coder *coder);

void		create_dongles(int n, t_dongle *dongles, char *arg);
void		create_shared(t_shared *shared);
void		give_values(t_coder *coders, t_dongle *dongle,
				t_shared *shared, char **args);
void		create_coders(t_coder *coders, t_dongle *dongle,
				t_shared *shared, char **args);

void		change_compiles(t_coder *coder);
long long	last_copile(t_coder *coder);
int			read_compiles(t_coder *coder);
void		set_simulation(t_shared *shared);

void		*monitor_func(void *arg);

t_request	*make_req(t_dongle *dong, t_coder *coder);
t_request	*best_req(t_dongle *dong);
int			take_a_dongle(t_dongle *dongle, t_coder *coder);

void		*rutine(void *arg);

#endif