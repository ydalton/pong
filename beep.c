#include <linux/kd.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "beep.h"

void beep(float freq, float length)
{
    ioctl(STDOUT_FILENO, KIOCSOUND, PCSPKR_MAGIC/freq);
    usleep(length * 1000);
    ioctl(STDOUT_FILENO, KIOCSOUND, 0); /*Stop silly sound*/
}
