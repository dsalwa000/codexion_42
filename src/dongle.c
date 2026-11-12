#include "codexion.h"

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

int	release_dongle(t_dongle *d)
{

}
