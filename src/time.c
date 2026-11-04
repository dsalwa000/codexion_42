#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	timev;
	
	gettimeofday(&timev, NULL);
	return ((long long)timev.tv_sec * 1000 + timev.tv_usec / 1000);
}

void	stop_therad_for(long long ms)
{
	long long	end;

	end = get_time_ms() + ms;
	while (get_time_ms() < end)
		usleep(500);
}
