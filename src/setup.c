#include "codexion.h"

static int	init_dongle(t_dongle *dongle)
{
	memset(dongle, 0, sizeof(*dongle));
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}

static int	init_coder(t_simulation *sim, int i)
{
	t_coder	*coder;

	coder = &sim->coders[i];
	memset(coder, 0, sizeof(*coder));
	coder->id = i + 1;
	coder->simulation = sim;
	if (i == sim->params.coders - 1)
	{
		coder->first = &sim->dongles[0];
		coder->second = &sim->dongles[i];
	}
	else
	{
		coder->first = &sim->dongles[i];
		coder->second = &sim->dongles[i + 1];
	}
	if (pthread_mutex_init(&coder->state_mutex, NULL) != 0)
		return (0);
	return (1);
}

static int	init_sync(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	if (pthread_mutex_init(&sim->start_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	if (pthread_cond_init(&sim->start_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->start_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		pthread_mutex_destroy(&sim->stop_mutex);
		return (0);
	}
	return (1);
}

static int	init_simulation(t_simulation *sim, t_params params)
{
	memset(sim, 0, sizeof(*sim));
	sim->params = params;
	if (!init_sync(sim))
		return (0);
	sim->dongles = malloc(sizeof(t_dongle) * params.coders);
	sim->coders = malloc(sizeof(t_coder) * params.coders);
	if (!sim->dongles || !sim->coders)
	{
		destroy_sim(sim, 0, 0);
		return (0);
	}
	return (1);
}

int	setup_simulation(t_simulation *sim, t_params params)
{
	int	i;

	if (!init_simulation(sim, params))
		return (0);
	i = 0;
	while (i < params.coders && init_dongle(&sim->dongles[i]))
		i++;
	if (i < params.coders)
	{
		destroy_sim(sim, i, 0);
		return (0);
	}
	i = 0;
	while (i < params.coders && init_coder(sim, i))
		i++;
	if (i < params.coders)
	{
		destroy_sim(sim, params.coders, i);
		return (0);
	}
	return (1);
}
