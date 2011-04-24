#include "signal_handling.h"

void (*trap(int signal, void (*func) (int signal_raised))) (int) { 
    struct sigaction new_action;
	struct sigaction old_action;

	memset(&new_action.sa_mask, 0, sizeof(new_action.sa_mask));

	new_action.sa_flags = 0;
	new_action.sa_handler = func;

	if (sigaction(signal, &new_action, &old_action) < 0) {
		return SIG_ERR;
	} else {
		return old_action.sa_handler;
	}
}
