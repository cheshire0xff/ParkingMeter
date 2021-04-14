/*
 * Debug.h
 *
 *  Created on: 14 Apr 2021
 *      Author: cheshire
 */

#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_
#include "stdio.h"


/*
 * todo
 */

struct Logger
{
    template<typename... Args>
    void logDebug(const char* pattern, Args&&... args)
    {
      printf("Dbg: ");
      printf(pattern, args...);
      printf("\n");
    }
    template<typename... Args>
    void logInfo(const char* pattern, Args&&... args)
    {
      printf("Inf: ");
      printf(pattern, args...);
      printf("\n");
    }
    template<typename... Args>
    void logError(const char* pattern, Args&&... args)
    {
      printf("Error: ");
      printf(pattern, args...);
      printf("\n");
    }
};


#endif /* INC_LOGGER_H_ */
