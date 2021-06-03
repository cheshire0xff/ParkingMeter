/*
 * Defs.h
 *
 *  Created on: 3 Jun 2021
 *      Author: cheshire
 */

#ifndef INC_DEFS_H_
#define INC_DEFS_H_

namespace Defs
{
enum InputValues
{
    none = 0,
    fifty_gr = 1,
    one_pln = 2,
    two_pln = 3,
    five_pln = 4,
    reset = 5,
};

unsigned int coinToGr(uint8_t coin)
{
    switch (coin)
    {
    case Defs::fifty_gr:
        return 50;
    case Defs::one_pln:
        return 100;
    case Defs::two_pln:
        return 200;
    case Defs::five_pln:
        return 500;
    default:
        return 0;
    }
}
const char* coinToString(uint8_t coin)
{
    switch (coin)
    {
    case Defs::none:
        return "none";
    case Defs::fifty_gr:
        return "50gr";
    case Defs::one_pln:
        return "1pln";
    case Defs::two_pln:
        return "2pln";
    case Defs::five_pln:
        return "5pln";
    }
    return "invalid";
}
unsigned int coinToMinutes(uint8_t coin)
{
    switch (coin)
    {
    case Defs::none:
        return 0;
    case Defs::fifty_gr:
        return 15;
    case Defs::one_pln:
        return 30;
    case Defs::two_pln:
        return 60;
    case Defs::five_pln:
        return 180;
    }
    return 0;
}
}; // namespace Defs

#endif /* INC_DEFS_H_ */
