#include "types.h"
#include "user.h"


int main(int argc, char **argv) {
	int ret = 0;

	// If the user gives 1 as parameter, then we call with reset and print the value
	if ('1' == *argv[1]) {
		printf(1, "Resetting...\n");
		ret = getreadcount(1);
	} else {
		ret = getreadcount(0);
	}
	
	// Just print it:
	printf(1, "Read count: %d\n", ret);	

	exit();
}
