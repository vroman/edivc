#include "qgraphics.h"

int ExportaFuncs(EXPORTAFUNCS_PARAMS)
{
	ENTRYPOINT(first_load);

	return TRUE;
}

void first_load(FUNCTION_PARAMS)
{
	gr_Init()
}

void frame(FUNCTION_PARAMS)