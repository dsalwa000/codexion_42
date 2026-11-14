#include "codexion.h"

int	all_compiled(t_simulation *sim)
{
	int	i;
	int	done;

	i = 0;
	while (i < sim->params.coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		done = sim->coders[i].compiles >= sim->params.compiles_required;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}

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
	t_simulation	*sim;
	long long		now;
	long long		last_compile;

	sim = coder->simulation;
	pthread_mutex_lock(&coder->state_mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	now = get_time_ms();
	if (now - last_compile <= sim->params.t_burnout)
		return (0);
	pthread_mutex_lock(&sim->log_mutex);
	if (!is_stopped(sim))
		printf("%lld %d burned out\n", now - sim->start, coder->id);
	set_stopped(sim, 1);
	pthread_mutex_unlock(&sim->log_mutex);
	return (1);
}
