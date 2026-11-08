#include "codexion.h"

static void	*monitor_thread(void *arg)
{
	int				i;
	t_simulation	*sim;

	sim = (t_simulation *)arg;

	while (!sim->stopped)
	{
		i = 0;
		while(i < sim->params.coders)
			if (check_burnout(&sim->coders[i]))
			{
				sim->stopped = 1;
				break;
			}
		usleep(CHECK_WAIT);
	}
	return (NULL);
}

static void	*coder_thread(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
}

void	start_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	pthread_create(&sim->monitor, NULL, monitor_thread, sim);
	while (i < sim->params.coders)
	{
		pthread_create(&sim->coders[i].thread, NULL, coder_thread, &sim->coders[i]);
		i++;
	}
}
