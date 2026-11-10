#include "codexion.h"

int	wait_for_start(t_simulation *sim)
{
	int	state;

	pthread_mutex_lock(&sim->start_mutex);
	while (sim->start_state == 0)
		pthread_cond_wait(&sim->start_cond, &sim->start_mutex);
	state = sim->start_state;
	pthread_mutex_unlock(&sim->start_mutex);
	return (state);
}
