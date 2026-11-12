#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>

# define NB_ARGS 8
# define QUEUE_SIZE 2
# define CHECK_WAIT 1000

typedef enum e_sched
{
	FIFO,
	EDF
}	t_sched;

typedef struct s_params
{
	int		coders;
	int		t_burnout;
	int		t_compile;
	int		t_debug;
	int		t_refactor;
	int		compiles_required;
	int		cooldown;
	t_sched	sched;
}	t_params;

typedef struct s_request
{
	int			coder_id;
	long long	priority;
}	t_request;

typedef struct s_queue
{
	t_request	requests[QUEUE_SIZE];
	int			in_line;
}	t_queue;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				is_held;
	t_queue			queue;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;
	t_dongle			*first;
	t_dongle			*second;
	long long			last_compile_start;
	int					compiles;
	pthread_mutex_t		state_mutex;
	struct s_simulation	*simulation;
}	t_coder;

typedef struct s_simulation
{
	t_params		params;
	long long		start;
	int				stopped;
	int				nb_coders;
	int				start_state;
	pthread_cond_t	start_cond;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	log_mutex;
	pthread_t		monitor;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_simulation;

int			parser(int argc, char **args, t_params *params);

long long	get_time_ms(void);
void		ft_sleep(t_simulation *sim, long long ms);

int			setup_simulation(t_simulation *sim, t_params params);

void		join_threads(t_simulation *sim);
int			abort_start(t_simulation *sim);
void		destroy_simulation(t_simulation *sim, int nb_dongles, int nb_coders);

void		print_status(t_coder *coder, char *msg);
int			print_error(char *msg);

int			start_simulation(t_simulation *sim);

int			is_stopped(t_simulation *sim);
void		set_stopped(t_simulation *sim, int stopped);
void		set_start_state(t_simulation *sim, int state);
int			check_burnout(t_coder *coder);

int			wait_for_start(t_simulation *sim);

int			heap_push(t_queue *q, t_request req);
int			heap_pop(t_queue *q, t_request *out);

void		swap_requests(t_request *a, t_request *b);
int			is_before(t_request *a, t_request *b);

t_request	create_request(t_coder *c);
void		add_request_to_queue(t_dongle *d, t_request req);
int			release_dongle(t_dongle *d);

#endif