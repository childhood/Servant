#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void (*trap(int signal, void (*func) (int signal_raised))) (int);
