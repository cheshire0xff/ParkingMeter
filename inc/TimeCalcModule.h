/*
 * TimeCalcModule.h
 *
 *  Created on: 14 Apr 2021
 *      Author: cheshire
 */

#ifndef INC_TIMECALCMODULE_H_
#define INC_TIMECALCMODULE_H_

#include "Logger.h"
#include <cstdint>
#include <systemc.h>

class TimeCalcModule : ::sc_core::sc_module
{
  public:
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<uint8_t> coin;

    sc_out<int> timeMinutes;
    TimeCalcModule(sc_core::sc_module_name)
    {
        sc_core::sc_process_handle calculate_handle = sc_core::sc_get_curr_simcontext()->create_method_process(
            "calculate", false, static_cast<sc_core::SC_ENTRY_FUNC>(&TimeCalcModule::calculate), this, nullptr);
        sensitive << calculate_handle;
        sensitive << clk.pos();
    }

    enum CoinT : uint8_t
    {
        none = 0,
        fifty_gr = 1,
        one_pln = 2,
        two_pln = 3,
        five_pln = 4
    };

  private:
    uint accumulatedTime = 0;
    void calculate()
    {
        auto logger = Logger{};
        logger.logDebug("calculate trg");
        logger.logDebug("clk: %d", clk.read());
        switch (coin.read())
        {
        case CoinT::none:
            break;
        case CoinT::fifty_gr:
            accumulatedTime += 15;
            break;
        case CoinT::one_pln:
            accumulatedTime += 30;
            break;
        case CoinT::two_pln:
            accumulatedTime += 60;
            break;
        case CoinT::five_pln:
            accumulatedTime += 180;
            break;
        default:
            // error
            break;
        }
        timeMinutes = accumulatedTime;
    }
};

#endif /* INC_TIMECALCMODULE_H_ */
