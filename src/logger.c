#include "codexion.h"

void	print_status(t_coder *coder, char *msg) {
	long long	timestamp;

	timestamp = get_time_ms() - coder->simulation->start;
	fprintf("%lln %d %s", timestamp, coder->id, msg);
}
