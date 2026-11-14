#include "codexion.h"

static void	*monitor_thread(void *arg)
{
	int				i;
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	if (wait_for_start(sim) == -1)
		return (NULL);
	while (!is_stopped(sim))
	{
		i = 0;
		while (i < sim->params.coders && !check_burnout(&sim->coders[i]))
			i++;
		if (i == sim->params.coders && all_compiled(sim))
			set_stopped(sim, 1);
		usleep(CHECK_WAIT);
	}
	return (NULL);
}

static void	*coder_thread(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;
	t_request		req;

	coder = (t_coder *)arg;
	sim = coder->simulation;
	if (wait_for_start(sim) == -1)
		return (NULL);
	if (coder->first == coder->second)
		return (lonely_coder(coder));
	while (!is_stopped(sim))
	{
		req = create_request(coder);
		add_request_to_queue(coder->first, req);
		add_request_to_queue(coder->second, req);
		if (!wait_for_dongles(coder))
			break;
		compile_debug_refactor(coder);
	}
	return (NULL);
}

static void	set_start_time(t_simulation *sim)
{
	int	i;

	i = 0;
	sim->start = get_time_ms();
	while (i < sim->params.coders)
		sim->coders[i++].last_compile_start = sim->start;
}

int	start_simulation(t_simulation *sim)
{
	int		i;
	t_coder	*coder;

	i = 0;
	while (i < sim->params.coders)
	{
		coder = &sim->coders[i];
		if (pthread_create(&coder->thread, NULL, coder_thread, coder))
			return (0);
		sim->nb_coders++;
		i++;
	}
	if (pthread_create(&sim->monitor, NULL, monitor_thread, sim) != 0)
		return (0);
	set_start_time(sim);
	set_start_state(sim, 1);
	join_threads(sim);
	return (1);
}
