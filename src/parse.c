/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 09:35:46 by jtruckse          #+#    #+#             */
/*   Updated: 2026/05/22 17:32:26 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	check_if_valid_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	process_args(char **av)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (!check_if_valid_num(av[i]))
		{
			error_m2(i);
			return (0);
		}
		if (atoi(av[i]) < 0)
		{
			error_m3();
		}
		i++;
	}
	if ((strcmp(av[i], "fifo") != 0) && (strcmp(av[i], "edf") != 0))
		error_m2(i);
	return (1);
}

/*das ist das beispiel von chatty, wie ich das mit der erstellung von den threads machen kann hier wird ein dynamisches array gemach und dann die anzahl der threads gemalloced nach dem die argumente geparst wurden,
so oder soo aehnlich muss ich das auch machen. 
in meinem parsing sind schon die nummern validiert und gecheck das diese richtig sind und auch brauchbar. nun muss ich noch checken ob die zeiten auch richtig sind oder nicht und ob es moeglich ist das mindestens ein thread den vorgang ausfuehren kann ohne zu sterben.
*/

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// void* worker(void* arg) {
//     int id = *(int*)arg;

//     printf("Thread %d läuft\n", id);

//     return NULL;
// }

// int main(int argc, char* argv[]) {
//     if(argc < 2)
//         return 1;

//     int n = atoi(argv[1]);

//     pthread_t* threads =
//         malloc(sizeof(pthread_t) * n);

//     int* ids =
//         malloc(sizeof(int) * n);

//     for(int i = 0; i < n; i++) {
//         ids[i] = i;

//         pthread_create(
//             &threads[i],
//             NULL,
//             worker,
//             &ids[i]
//         );
//     }

//     for(int i = 0; i < n; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     free(threads);
//     free(ids);

//     return 0;
// }