#ifndef __LOGGER__
#define __LOGGER__

#include <string.h>

namespace logger {
    /* Logger levels */
    enum {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    /* Logger level: default DEBUG */
    extern int level;

    /* Log messages destination: default stderr */
    extern FILE* output;

    /* Application Log function, prints the message only if priority is higher than the logger internal level */
    void log(int priority, const std::string& fmt, ...);
};

#endif // __LOGGER__
