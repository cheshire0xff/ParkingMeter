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

// todo error handling

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
    static const char* coinToString(uint8_t coin)
    {
        switch (coin)
        {
        case TimeCalcModule::none:
            return "none";
        case TimeCalcModule::fifty_gr:
            return "50gr";
        case TimeCalcModule::one_pln:
            return "1pln";
        case TimeCalcModule::two_pln:
            return "2pln";
        case TimeCalcModule::five_pln:
            return "5pln";
        }
        return "invalid";
    }
    static const unsigned int coinToMinutes(uint8_t coin)
    {
        switch (coin)
        {
        case TimeCalcModule::none:
            return 0;
        case TimeCalcModule::fifty_gr:
            return 15;
        case TimeCalcModule::one_pln:
            return 30;
        case TimeCalcModule::two_pln:
            return 60;
        case TimeCalcModule::five_pln:
            return 180;
        }
        return 0;
    }

  private:
    unsigned int accumulatedTime = 0;
    void calculate()
    {
        auto logger = DefaultLogger{};
        logger.debug("calculate trg");
        logger.debug("clk: %d", clk.read());
        auto val = coin.read();
        if (val <= CoinT::five_pln)
        {
            accumulatedTime += coinToMinutes(val);
        }
        else
        {
            // error
            logger.error("invalid coin: %d", coin.read());
        }
        timeMinutes = accumulatedTime;
    }
};

#endif /* INC_TIMECALCMODULE_H_ */
