// (c)2013, Arthur van Hoff

#ifndef __common_util__
#define __common_util__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <netinet/in.h>
#if ! (JUCE_IOS || JUCE_ANDROID)
    #include <vector_types.h>
    #include <vector_functions.h>
#endif

#define DEPRECATED __attribute__((deprecated))

//
// useful functions
//

template<class T> T min(T a, T b) { return a < b ? a : b; }
template<class T> T max(T a, T b) { return a > b ? a : b; }

inline bool startswith(const char *fname, const char *prefix)
{
    return strncmp(fname, prefix, strlen(prefix)) == 0;
}
inline bool endswith(const char *fname, const char *postfix)
{
    int len1 = strlen(fname);
    int len2 = strlen(postfix);
    return len1 >= len2 && strcmp(fname + len1 - len2, postfix) == 0;
}

/*
Given an amount of space (in bytes) and a bitrate (in bits/s) and an fps
generate the number of hours, minutes, seconds, and frames
*/
inline void calculateRecTimeLeft(const long space, const long bitRate, int *hours, int *minutes, int *seconds)
{
    if (bitRate <= 0 || space <= 0) {
        return;
    }
    
    long totalBits = space * 8;
    long oneSecondBits = bitRate;
    long oneMinuteBits = oneSecondBits * 60;
    long oneHourBits = oneMinuteBits * 60;

    *hours = totalBits / oneHourBits;
    totalBits = totalBits % oneHourBits;

    *minutes = totalBits / oneMinuteBits;
    totalBits = totalBits % oneMinuteBits;

    *seconds = totalBits / oneSecondBits;

    return;
}

extern bool __fatal_error(const char *file, int line, const char *func, const char *fmt, ...);

#define FATAL(...)           __fatal_error(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define ASSERT(check, ...)   ((check) || __fatal_error(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__))

//
// Time
//

inline long long getTimeNanos()
{
    timeval tm;
    gettimeofday(&tm, NULL);
    return tm.tv_sec * 1000000000LL + tm.tv_usec * 1000LL;
}

inline long long getTimeMillis()
{
    timeval tm;
    gettimeofday(&tm, NULL);
    return tm.tv_sec * 1000LL + tm.tv_usec / 1000LL;
}

inline long getTimeSeconds()
{
    timeval tm;
    gettimeofday(&tm, NULL);
    return tm.tv_sec;
}

inline double getSystemTime()
{
    return getTimeNanos() / 1000000000.0;
}

extern void atomic_add(volatile int &value, int amount);
extern void atomic_add(volatile long long &value, int amount);
extern void atomic_max(volatile int &value, int other);
extern void atomic_max(volatile long long &value, long long other);

//
// other includes
//

#if ! (JUCE_IOS || JUCE_ANDROID)
    #include "common/types.h"
#endif
#include "common/defs.h"

//
// Globals
//

extern bool interrupted;
extern const char *jaunt_version;

#endif
