/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 09:48:37 by jtruckse          #+#    #+#             */
/*   Updated: 2026/05/21 17:10:32 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	error_m1(void)
{
	printf("WRONG AMOUNT OF ARUGMENTS... EXPECTET:\n<number_of_coders>\n"
		"<time_to_burnout>\n<time_to_compile>\n<time_to_debug>\n"
		"<time_to_refactor>\n<number_of_compiles_required>\n<dongle_cooldown>"
		"\n<scheduler>\n");
	exit(1);
}

void	error_m2(int i)
{
	if (i == 1)
		printf("<numbers_of_coders> should be a number\n");
	else if (i == 2)
		printf("<time_to_bournout> should be a time (in milliseconds)\n");
	else if (i == 3)
		printf("<time_to_compile> should be a time (in milliseconds)\n");
	else if (i == 4)
		printf("<time_to_debug> should be a time (in milliseconds)\n");
	else if (i == 5)
		printf("<time_to_refactor> should be a time (in milliseconds)\n");
	else if (i == 6)
		printf("<number_of_compiles_required> should be a number\n");
	else if (i == 7)
		printf("<dogle_cooldown> should be a time (in milliseconds)\n");
	else if (i == 8)
		printf("schedule needs to be 'fifo' or 'edf'\n");
	exit(1);
}

void error_m3(void)
{
	printf("the numbers schould always be positive\n");
	exit(1);
}
