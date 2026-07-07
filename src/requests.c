/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtruckse <jtruckse@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 22:43:08 by jtruckse          #+#    #+#             */
/*   Updated: 2026/07/08 01:22:28 by jtruckse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

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
	pthread_mutex_unlock(&dongle->dongle);
	pthread_mutex_lock(&dongle->dongle);
	while ((dongle->taken == 1 || req != best_req(dongle))
		&& simulation(coder->shared))
		pthread_cond_wait(&dongle->d_cond, &dongle->dongle);
	if (dongle->waiting_count == 2 && dongle->waiting[0] == req)
		dongle->waiting[0] = dongle->waiting[1];
	dongle->waiting_count--;
	dongle->taken = 1;
	pthread_mutex_unlock(&dongle->dongle);
	if (!simulation(coder->shared))
	{
		if (dongle->waiting_count > 0)
			dongle->waiting_count--;
		free(req);
		pthread_mutex_unlock(&dongle->dongle);
		return (0);
	}
	free(req);
	return (1);
}
