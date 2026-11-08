#include "codexion.h"

int	main(int argc, char **argv)
{
	t_params		params;
	t_simulation	simulation;

	if (!parser(argc, argv + 1, &params))
	{
		fprintf(stderr, "Error: invalid arguments\n");
		return (1);
	}
	if (!(setup_simulation(&simulation, params)))
	{
		fprintf(stderr, "Error: simulation is not set properly\n");
		return (1); 
	}

	destroy_simulation(&simulation, params.coders, params.coders);
	return (0);
}
