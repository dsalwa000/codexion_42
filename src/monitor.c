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

void	set_start_state(t_simulation *sim, int state)
{
	pthread_mutex_lock(&sim->start_mutex);
	sim->start_state = state;
	pthread_mutex_unlock(&sim->start_mutex);
	pthread_cond_broadcast(&sim->start_cond);
}

int	check_burnout(t_coder *coder)
{
	int	used_time;
	int	burnout;

	used_time = get_time_ms() - coder->last_compile_start;
	burnout = coder->simulation->params.t_burnout;
	if (used_time > burnout)
		return (1);
	return (0);
}
