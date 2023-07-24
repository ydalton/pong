#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/input.h>

#include "beep.h"

int beep(float freq, float length)
{
    int fd = open(BEEP_FD, O_WRONLY);
    if(fd == -1) {
        perror("open");
        return -1;
    }
	if(fd == -1) {
		perror("open");
	}
	struct input_event e = {
		.type = EV_SND,
		.code = SND_TONE,
		.value = freq,
	};
	if (sizeof(e) != write(fd, &e, sizeof(e))) {
		perror("write:");
		return -1;
    }
    usleep(length * 1000);
	e.value = 0;
	if (sizeof(e) != write(fd, &e, sizeof(e))) {
		perror("write:");
		return -1;
    }
    close(fd);
    return 0;
}
