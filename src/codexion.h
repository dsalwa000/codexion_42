#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define NB_ARGS 8

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

int			parser(int argc, char **args, t_params *params);

long long	get_time_ms(void);
void		stop_therad_for(long long ms);


#endif