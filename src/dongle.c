#include "codexion.h"

static int	can_take(t_dongle *d, int coder_id, long long now)
{
	return (!d->is_held && now >= d->available_at
			&& d->queue.in_line > 0
			&& d->queue.requests[0].coder_id == coder_id);
}

t_request	create_request(t_coder *c)
{
	t_request	req;

	req.coder_id = c->id;
	pthread_mutex_lock(&c->state_mutex);
	if (c->simulation->params.sched == FIFO)
		req.priority = get_time_ms();
	else
		req.priority = c->last_compile_start;
	pthread_mutex_unlock(&c->state_mutex);
	return (req);
}

void	add_request_to_queue(t_dongle *d, t_request req)
{
	pthread_mutex_lock(&d->mutex);
	heap_push(&d->queue, req);
	pthread_mutex_unlock(&d->mutex);
}

int	start_compiling(t_dongle *first, t_dongle *second, int coder_id)
{
	long long	now;
	int			result;

	result = 0;
	now = get_time_ms();
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	if (can_take(first, coder_id, now) && can_take(second, coder_id, now))
	{
		first->is_held = 1;
		second->is_held = 1;
		heap_pop(&first->queue, NULL);
		heap_pop(&second->queue, NULL);
		result = 1;
	}
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (result);
}
