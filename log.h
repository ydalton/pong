#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#define _eprintf(fmt, ...) fprintf (stderr, fmt, ##__VA_ARGS__)
#define _LOG(fmt, ...) _eprintf("%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_I(fmt, ...) _LOG("info: " fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...) _LOG("warning: " fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) _LOG("error: " fmt, ##__VA_ARGS__)

#endif // LOG_H_
