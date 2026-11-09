#include "codexion.h"

int	is_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->stop_mutex);
	stopped = sim->stopped;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stopped);
}

void	set_stopped(t_simulation *sim, int stopped)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stopped = stopped;
	pthread_mutex_unlock(&sim->stop_mutex);
}

int	check_burnout(t_coder *coder)
{
	int used_time;
	int burnout;

	used_time = get_time_ms() - coder->last_compile_start;
	burnout = coder->simulation->params.t_burnout;
	if (used_time > burnout)
		return (0);
	return (1);
}
