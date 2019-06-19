#include <cstdio>
#include <cstdarg>
#include <string>

#include "logger.h"

int logger::level = logger::DEBUG;

FILE* logger::output = stderr;

void logger::log(int priority, const std::string& fmt, ...){
    va_list vl;
    va_start(vl, fmt);

    if (priority >= logger::level) {
        fprintf(logger::output, fmt.c_str(), vl);
    }

    va_end(vl);
}

