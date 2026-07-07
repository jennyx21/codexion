#include "../codexion.h"

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

