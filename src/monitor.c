#include "codexion.h"

int	check_burnout(t_coder *coder)
{
	int used_time;
	int burnout;

	used_time = get_time_ms() - coder->last_compile_start;
	burnout = coder->simulation->params.t_burnout;
	if (used_time > burnout)
		return (0);
	return (1);
}
