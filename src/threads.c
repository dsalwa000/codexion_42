#include "codexion.h"

static void	*monitor_thread(void *arg)
{
	int				i;
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	sim->start = get_time_ms();
	while (!is_stopped(sim))
	{
		i = 0;
		while(i < sim->params.coders)
		{
			if (check_burnout(&sim->coders[i]))
			{
				set_stopped(sim, 1);
				break;
			}
			i++;
		}
		usleep(CHECK_WAIT);
	}
	return (NULL);
}

static void	*coder_thread(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;

	return (NULL);
}

int	start_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	if (pthread_create(&sim->monitor, NULL, monitor_thread, sim) != 0)
		return (0);
	sim->monitor_created = 1;
	while (i < sim->params.coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_thread, &sim->coders[i]))
			return (0);
		sim->nb_coders++;
		i++;
	}
	join_threads(sim);
	return (1);
}
