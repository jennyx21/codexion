/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:25:44 by jtruckse          #+#    #+#             */
/*   Updated: 2026/06/30 00:58:49 by jtruckse         ###   ########.fr       */
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

typedef struct s_shared
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int					nb;
	char				*schedule;
	int					arived;
	long long			start_time;
	bool				simulation;
}	t_shared;

typedef struct s_dongle
{
	int					id;
	pthread_mutex_t		dongle;
	long long			cooldown;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				arival_id;
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