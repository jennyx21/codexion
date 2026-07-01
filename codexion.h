/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:25:44 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/01 05:15:17 by jtruckse         ###   ########.fr       */
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
	long long		timestamp;
	pthread_mutex_t	req_mutext;
}	t_request;

typedef struct s_queue
{
	int			q_id;
	
};


typedef struct s_shared
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int					nb;
	char				*schedule;
	int					arived;
	long long			start_time;
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
	int				compiles_done;
}	t_coder;

typedef struct s_monitor
{
	pthread_t		monitor;
	t_coder			*coder;
	pthread_cond_t	m_cond;
}	t_monitor;

int		process_args(char **av);

void	error_m1(void);
void	error_m2(int i);
void	error_m3(void);

#endif