/*
 * InputValidationModule.h
 *
 *  Created on: 2 Jun 2021
 *      Author: cheshire
 */

#ifndef INC_INPUTVALIDATIONMODULE_H_
#define INC_INPUTVALIDATIONMODULE_H_

#include "Logger.h"
#include "TimeCalcModule.h"
#include <cstdint>
#include <systemc.h>

/*
 * checks if uint8_t entered value is recognized value
 */

class InputValidationModule : ::sc_core::sc_module
{
  public:
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<uint8_t> inputValue;

    sc_out<bool> isValid;
    InputValidationModule(sc_core::sc_module_name)
    {
        sc_core::sc_process_handle calculate_handle = sc_core::sc_get_curr_simcontext()->create_method_process(
            "validate", false, static_cast<sc_core::SC_ENTRY_FUNC>(&InputValidationModule::validate), this, nullptr);
        sensitive_pos << calculate_handle;
        sensitive_neg << calculate_handle;
        sensitive << calculate_handle;
        sensitive << clk.pos();
    }

  private:
    void validate()
    {
        auto logger = DefaultLogger{};
        logger.debug("validate trg");
        logger.debug("clk: %d", clk.read());
        if (reset.read())
        {
            isValid = false;
        }
        else
        {
            if (inputValue.read() == Defs::none)
            {
                isValid = false;
            }
            else if (inputValue.read() > Defs::reset)
            {
                isValid = false;
            }
            else
            {
                isValid = true;
            }
        }
    }
};

#endif /* INC_INPUTVALIDATIONMODULE_H_ */
