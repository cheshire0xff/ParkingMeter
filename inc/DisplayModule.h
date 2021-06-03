/*
 * DisplayModule.h
 *
 *  Created on: 2 Jun 2021
 *      Author: cheshire
 */

#ifndef INC_DISPLAYMODULE_H_
#define INC_DISPLAYMODULE_H_

#include "Logger.h"
#include "TimeCalcModule.h"
#include <cstdint>
#include <stdio.h>
#include <systemc.h>

// todo error handling

class DisplayModule : ::sc_core::sc_module
{
  public:
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<uint8_t> inputValueType;
    sc_in<int> inputValue;
    enum class InputValueType
    {
        none,
        minutes,
        coin,
        gr,
        reset,
        inputError,
    };

    DisplayModule(sc_core::sc_module_name)
    {
        sc_core::sc_process_handle calculate_handle = sc_core::sc_get_curr_simcontext()->create_method_process(
            "print", false, static_cast<sc_core::SC_ENTRY_FUNC>(&DisplayModule::printVal), this, nullptr);
        sensitive_pos << calculate_handle;
        sensitive_neg << calculate_handle;
        sensitive << calculate_handle;
        sensitive << clk.pos();
    }

  private:
    void printVal()
    {
        auto logger = DefaultLogger{};
        logger.debug("printVal trg");
        logger.debug("clk: %d", clk.read());
        switch (static_cast<InputValueType>(inputValueType.read()))
        {
        case InputValueType::none:
            break;
        case InputValueType::reset:
            puts("Resetting.");
            break;
        case InputValueType::gr:
            printf("Total money: %d.%d pln\n", inputValue.read() / 100, inputValue.read() % 100);
            break;
        case InputValueType::minutes:
            printf("Accumulated time: %d minutes\n", inputValue.read());
            break;
        case InputValueType::coin:
            printf("InsertedCoin: %s\n", Defs::coinToString(inputValue.read()));
            break;
        case InputValueType::inputError:
            printf("Error processing input: %d\n", inputValue.read());
            break;
        }
    }
};

#endif /* INC_DISPLAYMODULE_H_ */
