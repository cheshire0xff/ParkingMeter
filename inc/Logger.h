/*
 * Debug.h
 *
 *  Created on: 14 Apr 2021
 *      Author: cheshire
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_
#include "Config.h"
#include "stdio.h"

template <bool enable>
struct Logger
{
};
using DefaultLogger = Logger<Config::logsEnabled>;

template <>
struct Logger<false>
{
    template <typename... Args>
    void debug(const char* pattern, Args&&... args)
    {
    }
    template <typename... Args>
    void info(const char* pattern, Args&&... args)
    {
    }
    template <typename... Args>
    void error(const char* pattern, Args&&... args)
    {
    }
};

template <>
struct Logger<true>
{
    template <typename... Args>
    void debug(const char* pattern, Args&&... args)
    {
        printf("Dbg: ");
        printf(pattern, args...);
        printf("\n");
    }
    template <typename... Args>
    void info(const char* pattern, Args&&... args)
    {
        printf("Inf: ");
        printf(pattern, args...);
        printf("\n");
    }
    template <typename... Args>
    void error(const char* pattern, Args&&... args)
    {
        printf("Error: ");
        printf(pattern, args...);
        printf("\n");
    }
};

#endif /* INC_LOGGER_H_ */
