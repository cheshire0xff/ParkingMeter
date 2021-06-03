/*
 * TimeCalcModule.h
 *
 *  Created on: 14 Apr 2021
 *      Author: cheshire
 */

#ifndef INC_TIMECALCMODULE_H_
#define INC_TIMECALCMODULE_H_

#include "Defs.h"
#include "Logger.h"
#include <cstdint>
#include <systemc.h>

/*
 * calculates how many minutes you get
 * from amount of coins you put
 * (5x1pln is equivalent to 1x5pln etc.)
 */

class TimeCalcModule : ::sc_core::sc_module
{
  public:
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<uint8_t> coin;

    sc_out<int> timeMinutes;
    sc_out<int> accumulatedGr;
    TimeCalcModule(sc_core::sc_module_name)
    {
        sc_core::sc_process_handle calculate_handle = sc_core::sc_get_curr_simcontext()->create_method_process(
            "calculate", false, static_cast<sc_core::SC_ENTRY_FUNC>(&TimeCalcModule::calculate), this, nullptr);
        sensitive_pos << calculate_handle;
        sensitive_neg << calculate_handle;
        sensitive << calculate_handle;
        sensitive << clk.pos();
    }

  private:
    unsigned int accumulatedGrRegister = 0;
    unsigned int getTimeMinutes()
    {
        auto accumulatedTime = 0U;
        auto accumulatedGrTemp = accumulatedGrRegister;
        auto logger = DefaultLogger{};
        for (auto coin = Defs::five_pln; coin >= Defs::fifty_gr; coin = static_cast<Defs::InputValues>(coin - 1))
        {
            auto grPerCoin = Defs::coinToGr(coin);
            auto coinCount = accumulatedGrTemp / grPerCoin;
            logger.debug("%s , count: %d", Defs::coinToString(coin), coinCount);
            if (coinCount)
            {
                auto minPerCoin = Defs::coinToMinutes(coin);
                accumulatedTime += minPerCoin * coinCount;
                // remove money already converted to minutes from loop
                accumulatedGrTemp -= coinCount * grPerCoin;
            }
        }
        if (accumulatedGrTemp)
        {
            DefaultLogger{}.error("Calculation error: remaining gr: %d", accumulatedGrTemp);
        }
        return accumulatedTime;
    }
    void calculate()
    {
        auto logger = DefaultLogger{};
        logger.debug("calculate trg");
        logger.debug("clk: %d", clk.read());
        if (reset.read())
        {
            accumulatedGrRegister = 0;
        }
        else
        {
            auto val = coin.read();
            if (val <= Defs::five_pln)
            {
                accumulatedGrRegister += Defs::coinToGr(static_cast<Defs::InputValues>(val));
            }
            else
            {
                // error
                logger.error("invalid coin: %d", coin.read());
            }
        }
        accumulatedGr = accumulatedGrRegister;
        timeMinutes = getTimeMinutes();
    }
};

#endif /* INC_TIMECALCMODULE_H_ */
