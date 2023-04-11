#include "timeticks.h"
#include "private/decoder.h"

bool osnmp::decode(const RawData &data, TimeTicks &value)
{
    return decodeUnsigned(data, value);
}

bool osnmp::encode(TimeTicks value, RawData &data)
{
    return encodeUnsigned(osnmp::kTimeTicksId, value, data);
}

#ifdef _WIN32
#include <windows.h>
osnmp::TimeTicks osnmp::TimeTicks::upTime()
{
    return GetTickCount() / 10;
}

#elif __linux__ || __NetBSD__
#include <stdio.h>
osnmp::TimeTicks osnmp::TimeTicks::upTime()
{
    FILE* file = fopen("/proc/uptime", "r");

    if (file == NULL)
        return 0;

    double uptime;
    fscanf(file, "%lf", &uptime);
    fclose(file);

    return uptime * 100;
}

#elif __FreeBSD__ || __APPLE__
#include <time.h>
osnmp::TimeTicks osnmp::TimeTicks::upTime()
{
    struct timespec time_spec;

    if (clock_gettime(GETTIME_OPTION, &time_spec) != 0)
        return 0;

    uint64_t uptime = time_spec.tv_sec * 1000 + time_spec.tv_nsec / 1000000;
    return uptime / 10;
}

#elif __OpenBSD__
#include <stdio.h>
#include <time.h>
#include <sys/sysctl.h>
osnmp::TimeTicks osnmp::TimeTicks::upTime()
{
    struct timeval time_val;
    size_t len = sizeof(time_val);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};

    if (sysctl(mib, 2, &time_val, &len, NULL, 0) != 0)
        return 0;

    uint64_t boottime = time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
    uint64_t now = (uint64_t)time(NULL) * 1000;

    uint64_t uptime = now - boottime;
    return uptime / 10;
}
#endif


