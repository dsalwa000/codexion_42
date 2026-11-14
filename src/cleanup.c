#include "codexion.h"

void	join_threads(t_simulation *sim)
{
	while (sim->nb_coders > 0)
	{
		sim->nb_coders--;
		pthread_join(sim->coders[sim->nb_coders].thread, NULL);
	}
	if (sim->start_state == 1)
		pthread_join(sim->monitor, NULL);
}

int	abort_start(t_simulation *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stopped = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	set_start_state(sim, -1);
	join_threads(sim);
	destroy_sim(sim, sim->params.coders, sim->params.coders);
	return (0);
}

void	destroy_sim(t_simulation *sim, int nb_dongles, int nb_coders)
{
	int	i;

	i = 0;
	while (i < nb_dongles)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	i = 0;
	while (i < nb_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].state_mutex);
		i++;
	}
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->start_mutex);
	pthread_cond_destroy(&sim->start_cond);
	free(sim->dongles);
	free(sim->coders);
}
