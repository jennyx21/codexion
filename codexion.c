/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:24:43 by jtruckse          #+#    #+#             */
/*   Updated: 2026/06/05 23:19:37 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void* worker(void* arg) {
    int id = *(int*)arg;

    printf("Thread %d läuft\n", id);

    return NULL;
}

int	main(int ac, char **av)
{
	int			n;
	int			*id;
	pthread_t	*coders;
	int			i;

	i = 0;
	if (ac == 9)
		process_args(av);
	else
		error_m1();
	n = atoi(av[1]);
	coders = malloc(sizeof(pthread_t) * (n + 1));
	id = malloc(sizeof(int) * (n + 1));
	if (!coders || !id)
		return (0);
	while (i <= n)
	{
		id[i] = i;
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_create(&coders[i], NULL, worker, &id[i]);
		i++;
	}
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