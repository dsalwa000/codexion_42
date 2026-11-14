#include "codexion.h"

void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	is_before(t_request *a, t_request *b)
{
	if (a->priority != b->priority)
		return (a->priority < b->priority);
	if (a->coder_id % 2 != b->coder_id % 2)
		return (a->coder_id % 2 == 1);
	return (a->coder_id < b->coder_id);
}
