#include "codexion.h"

static int	parse_int(const char *str, int *out)
{
	int	nb;
	int	digit;
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		digit = str[i] - '0';
		if (nb > (INT_MAX - digit) / 10)
			return (0);
		nb = nb * 10 + digit;
		i++;
	}
	if (str[i] != '\0')
		return (0);
	*out = nb;
	return (1);
}

static int	parse_sched(const char *str, t_sched *out)
{
	if (!str)
		return (0);
	if (strcmp(str, "fifo") == 0)
		*out = FIFO;
	else if (strcmp(str, "edf") == 0)
		*out = EDF;
	else
		return (0);
	return (1);
}

static void	fill_params(t_params *params, const int *values)
{
	params->coders = values[0];
	params->t_burnout = values[1];
	params->t_compile = values[2];
	params->t_debug = values[3];
	params->t_refactor = values[4];
	params->compiles_required = values[5];
	params->cooldown = values[6];
}

int	parser(int argc, char **args, t_params *params)
{
	int	values[NB_ARGS - 1];
	int	i;

	if (argc != NB_ARGS + 1 || !args || !params)
		return (0);
	i = 0;
	while (i < NB_ARGS - 1)
	{
		if (!parse_int(args[i], &values[i]))
			return (0);
		if (i == 0 && values[i] < 1)
			return (0);
		i++;
	}
	if (!parse_sched(args[NB_ARGS - 1], &params->sched))
		return (0);
	fill_params(params, values);
	return (1);
}
