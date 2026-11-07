#include "codexion.h"

void	destroy_simulation(t_simulation *sim, int nb_dongles, int nb_coders)
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
	free(sim->dongles);
	free(sim->coders);
}
