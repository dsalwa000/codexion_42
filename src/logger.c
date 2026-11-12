#include "codexion.h"

void	print_status(t_coder *coder, char *msg)
{
	long long		timestamp;
	t_simulation	*sim;

	sim = coder->simulation;
	timestamp = get_time_ms() - sim->start;
	pthread_mutex_lock(&sim->log_mutex);
	if (!is_stopped(sim))
		printf("%lld %d %s\n", timestamp, coder->id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

int	print_error(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	return (1);
}
