#include "codexion.h"

void	print_status(t_coder *coder, char *msg)
{
	long long	timestamp;

	timestamp = get_time_ms() - coder->simulation->start;
	printf("%lld %d %s\n", timestamp, coder->id, msg);
}

int	print_error(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	return (1);
}
