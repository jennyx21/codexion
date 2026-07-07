#include "../codexion.h"


void	*monitor_func(void *arg)
{
	t_monitor		*monitor;
	t_coder			*coder;
	pthread_mutex_t	*print;

	monitor = (t_monitor *)arg;
	coder = monitor->coder;
	print = &coder->shared->print;
	while (simulation(coder->shared) == true)
	{
		if ((get_time() - last_copile(coder) > coder->time_to_burnout)
			&& read_compiles(coder) != 0)
		{
			pthread_mutex_lock(print);
			printf("%lld coder %d burnedout\n",
				get_time() - coder->shared->go_t, coder->id);
			pthread_mutex_unlock(print);
			set_simulation(coder->shared);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
