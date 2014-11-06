#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libfoo.h"

int main(int argc, char *argv[])
{
	printf("----%s---\n", __FUNCTION__);
	lib_a();
	lib_b();
	printf("---Done---\n");

	return 0;
}
