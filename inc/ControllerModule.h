/*
 * ControllerModule.h
 *
 *  Created on: 2 Jun 2021
 *      Author: cheshire
 */

#ifndef INC_CONTROLLERMODULE_H_
#define INC_CONTROLLERMODULE_H_

#include "DisplayModule.h"
#include "Logger.h"
#include <cstdint>
#include <stdio.h>
#include <systemc.h>

// todo error handling

class ControllerModule : ::sc_core::sc_module
{
  public:
    sc_in<bool> clk;
    sc_in<uint8_t> input;
    sc_in<bool> inputActive;

    sc_out<bool> reset;

    sc_out<uint8_t> timeCalcCoinInput;
    sc_in<int> timeCalcMinutesOutput;
    sc_in<int> timeCalcGrOutput;

    sc_out<uint8_t> inputValidationValueInput;
    sc_in<bool> inputValidationIsValidOutput;

    sc_out<uint8_t> displayValueTypeInput;
    sc_out<int> displayValueInput;

    enum class State
    {
        idle,
        validatingInput,
        calculating,
        result,
        resetting,
        error,
    } _state = State::idle;
    uint8_t inputRegister = 0;

    ControllerModule(sc_core::sc_module_name)
    {
        sc_core::sc_process_handle calculate_handle = sc_core::sc_get_curr_simcontext()->create_method_process(
            "validate", false, static_cast<sc_core::SC_ENTRY_FUNC>(&ControllerModule::processInput), this, nullptr);
        sensitive_pos << calculate_handle;
        sensitive_neg << calculate_handle;
        sensitive << calculate_handle;
        sensitive << clk.neg();
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
    uint8_t previousType = 0;
    int previousValue = 0;
    void processInput()
    {
        auto logger = DefaultLogger{};
        logger.debug("Process Input trg");
        logger.debug("clk: %d", clk.read());
        auto displayValueType = DisplayModule::InputValueType::none;
        switch (_state)
        {
        case State::idle:
            if (inputActive.read())
            {
                inputRegister = input.read();
                inputValidationValueInput = inputRegister;
                logger.debug("Controller: validating");
                _state = State::validatingInput;
            }
            break;
        case State::validatingInput:
            displayValueInput = inputRegister;
            if (inputValidationIsValidOutput.read())
            {
                if (inputRegister == Defs::reset)
                {
                    displayValueType = DisplayModule::InputValueType::reset;
                    logger.debug("Controller: resetting");
                    reset = true;
                    _state = State::resetting;
                }
                else
                {
                    timeCalcCoinInput = inputRegister;
                    logger.debug("Controller: calculating");
                    displayValueType = DisplayModule::InputValueType::coin;
                    _state = State::calculating;
                }
            }
            else
            {
                logger.debug("Controller: invalid input");
                displayValueType = DisplayModule::InputValueType::inputError;
                _state = State::error;
            }
            break;
        case State::calculating:
            displayValueInput = timeCalcMinutesOutput;
            displayValueType = DisplayModule::InputValueType::minutes;
            logger.debug("Controller: calculation completed");
            timeCalcCoinInput = Defs::none;
            _state = State::result;
            break;
        case State::result:
            displayValueInput = timeCalcGrOutput;
            displayValueType = DisplayModule::InputValueType::gr;
            _state = State::idle;
            break;
        case State::error:
            _state = State::idle;
            break;
        case State::resetting:
            reset = false;
            _state = State::idle;
            break;
        }
        displayValueTypeInput = static_cast<uint8_t>(displayValueType);
    }
};

#endif /* INC_CONTROLLERMODULE_H_ */
