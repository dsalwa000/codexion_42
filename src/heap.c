#include "codexion.h"

static void	sift_up(t_queue *q, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!is_before(&q->requests[i], &q->requests[parent]))
			break ;
		swap_requests(&q->requests[i], &q->requests[parent]);
		i = parent;
	}
}

static void	sift_down(t_queue *q, int i)
{
	int	best;
	int	left;
	int	right;

	while (1)
	{
		best = i;
		left = 2 * i + 1;
		right = 2 * i + 2;
		if (left < q->in_line
			&& is_before(&q->requests[left], &q->requests[best]))
			best = left;
		if (right < q->in_line
			&& is_before(&q->requests[right], &q->requests[best]))
			best = right;
		if (best == i)
			break ;
		swap_requests(&q->requests[i], &q->requests[best]);
		i = best;
	}
}

int	heap_push(t_queue *q, t_request req)
{
	if (q->in_line == QUEUE_SIZE)
		return (0);
	q->requests[q->in_line] = req;
	sift_up(q, q->in_line);
	q->in_line++;
	return (1);
}

int	heap_pop(t_queue *q, t_request *out)
{
	if (q->in_line == 0)
		return (0);
	if (out)
		*out = q->requests[0];
	q->in_line--;
	q->requests[0] = q->requests[q->in_line];
	sift_down(q, 0);
	return (1);
}
