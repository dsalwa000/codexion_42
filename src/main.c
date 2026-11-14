#include "codexion.h"

int	main(int argc, char **argv)
{
	t_params		params;
	t_simulation	simulation;

	if (!parser(argc, argv + 1, &params))
		return (print_error("invalid arguments"));
	if (!setup_simulation(&simulation, params))
		return (print_error("simulation is not set properly"));
	if (!start_simulation(&simulation))
	{
		abort_start(&simulation);
		return (print_error("thread creation error"));
	}
	destroy_sim(&simulation, params.coders, params.coders);
	return (0);
}
