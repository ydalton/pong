#ifndef BEEP_H_
#define BEEP_H_

#define BEEP_FD "/dev/input/by-path/platform-pcspkr-event-spkr"
#define BEEP_FREQ 490
#define BEEP_LEN 25

int beep(float freq, float length);

#endif // BEEP_H_
