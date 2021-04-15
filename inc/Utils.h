/*
 * Utils.h
 *
 *  Created on: 15 Apr 2021
 *      Author: cheshire
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_
#include <stdio.h>

class ConsoleOut
{
  public:
    ConsoleOut(size_t& tick) : tick(tick)
    {
    }
    template <typename... Args>
    void print(const char* pattern, Args&&... args)
    {
        printf("t - %d: ", tick);
        printf(pattern, args...);
        puts("");
    }

  private:
    size_t& tick;
};

#endif /* INC_UTILS_H_ */
