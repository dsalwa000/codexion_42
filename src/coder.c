#include "codexion.h"

static void	release_dongles(t_coder *coder)
{
	long long	available_at;
	t_dongle	*first;
	t_dongle	*second;

	first = coder->first;
	second = coder->second;
	available_at = get_time_ms() + coder->simulation->params.cooldown;
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	first->is_held = 0;
	second->is_held = 0;
	first->available_at = available_at;
	second->available_at = available_at;
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}

int	wait_for_dongles(t_coder *coder)
{
	while (!start_compiling(coder->first, coder->second, coder->id))
	{
		if (is_stopped(coder->simulation))
			return (0);
		usleep(100);
	}
	return (1);
}

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

void	compile_debug_refactor(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->simulation;
	print_status(coder, "has taken a dongle");
	print_status(coder, "has taken a dongle");
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	print_status(coder, "is compiling");
	ft_sleep(sim, sim->params.t_compile);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compiles++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongles(coder);
	print_status(coder, "is debugging");
	ft_sleep(sim, sim->params.t_debug);
	print_status(coder, "is refactoring");
	ft_sleep(sim, sim->params.t_refactor);
}

void	*lonely_coder(t_coder *coder)
{
	print_status(coder, "has taken a dongle");
	while (!is_stopped(coder->simulation))
		usleep(500);
	return (NULL);
}
