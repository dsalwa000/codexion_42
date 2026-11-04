#include "codexion.h"

int	main(int argc, char **argv)
{
	t_params	params;

	if (!parser(argc, argv + 1, &params))
	{
		fprintf(stderr, "Error: invalid arguments\n");
		return (1);
	}
	return (0);
}
