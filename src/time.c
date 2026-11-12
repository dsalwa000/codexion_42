#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	timev;
	
	gettimeofday(&timev, NULL);
	return ((long long)timev.tv_sec * 1000 + timev.tv_usec / 1000);
}

void	ft_sleep(t_simulation *sim, long long ms)
{
	long long	end;

	end = get_time_ms() + ms;
	while (get_time_ms() < end && !is_stopped(sim))
		usleep(500);
}
