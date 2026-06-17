/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:25:44 by jtruckse          #+#    #+#             */
/*   Updated: 2026/06/17 09:36:53 by jtruckse         ###   ########.fr       */
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

// stuct fuer die coders 
/*  id 
    time to burn out
    time from las compile 
    alive ?
    */

typedef struct s_dongle
{
	int					id;
	pthread_mutex_t		dongle;
	long long			cooldown;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		coder;
	t_dongle		left_dongle;
	t_dongle		right_dognle;
	long long		time_to_bournout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	struct timeval	start_time;
	struct timeval	new_death_timer;
	int				compiles_required;
	char			*schedule;
	bool			alive;
}	t_coder;


// stuckt monitor
/* bool everyone alavie?*/
int		process_args(char **av);

void	error_m1(void);
void	error_m2(int i);
void	error_m3(void);

#endif