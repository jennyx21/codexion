/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:25:44 by jtruckse          #+#    #+#             */
/*   Updated: 2026/05/15 14:46:31 by jtruckse         ###   ########.fr       */
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


int		process_args(char **av);

void	error_m1(void);
void	error_m2(int i);

#endif