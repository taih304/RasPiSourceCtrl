#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
	struct winsize ws;//defined in sys/ioctl
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);

	printf("file: %d\n", STDIN_FILENO);
	printf("rows: %d\n", ws.ws_row);
	printf("columns: %d\n", ws.ws_col);

	return (0);
}