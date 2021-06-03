#include <stdio.h>
#include <systemc.h>

#include "ControllerModule.h"
#include "DisplayModule.h"
#include "InputValidationModule.h"
#include "Logger.h"
#include "TimeCalcModule.h"
#include "Utils.h"
#include "atomic"
#include <thread>

using namespace std;
std::atomic<bool> consoleInputFlag;
std::atomic<int> consoleInputValue;

void pollStdin()
{
    std::string str;
    while (true)
    {
        std::getline(std::cin, str);
        if (str.size() == 1 && std::isdigit(str[0]))
        {
            consoleInputValue.store(std::stoi(str));
            consoleInputFlag.store(true);
        }
        else
        {
            std::cout << "Only single digit input is allowed!\n";
        }
    }
};

int sc_main(int, char**)
{
    printf("Usage: \n");
    {
        for (auto coin = Defs::fifty_gr; coin < Defs::reset; coin = static_cast<Defs::InputValues>(coin + 1))
        {
            printf("\t%d - %s(%d min)\n", coin, Defs::coinToString(coin), Defs::coinToMinutes(coin));
        }
        printf("\t 5 - reset\n");
    }
    consoleInputFlag.store(false);
    sc_signal<bool> clk;
    sc_signal<uint8_t> input;

    sc_signal<bool> isValid;
    sc_signal<bool> reset;
    sc_signal<bool> inputActive;
    sc_signal<uint8_t> inputValue;
    sc_signal<uint8_t> coin;
    sc_signal<int> timeMinutes;
    sc_signal<int> moneyTotalGr;
    sc_signal<uint8_t> inputValueType;
    sc_signal<int> displayInput;

    ControllerModule controller("Controller");
    TimeCalcModule timeCalc("TimeCalc");
    InputValidationModule inputValid("inputValid");
    DisplayModule display("display");

    controller.clk(clk);
    controller.input(input);
    controller.inputActive(inputActive);

    controller.reset(reset);

    controller.inputValidationIsValidOutput(isValid);
    controller.inputValidationValueInput(inputValue);

    controller.timeCalcCoinInput(coin);
    controller.timeCalcMinutesOutput(timeMinutes);
    controller.timeCalcGrOutput(moneyTotalGr);

    controller.displayValueInput(displayInput);
    controller.displayValueTypeInput(inputValueType);

    timeCalc.clk(clk);
    timeCalc.reset(reset);
    timeCalc.coin(coin);
    timeCalc.timeMinutes(timeMinutes);
    timeCalc.accumulatedGr(moneyTotalGr);

    inputValid.clk(clk);
    inputValid.reset(reset);
    inputValid.isValid(isValid);
    inputValid.inputValue(inputValue);

    display.clk(clk);
    display.reset(reset);
    display.inputValueType(inputValueType);
    display.inputValue(displayInput);

    clk = false;

    sc_time tickHalfTime{1, SC_MS};
    size_t tick = 0;
    ConsoleOut console{tick};

    auto incrementTick = [&] {
        clk = true;
        sc_start(tickHalfTime);
        ++tick;
        clk = false;
        sc_start(tickHalfTime);
    };
    auto insertValue = [&](Defs::InputValues coin) {
        input = coin;
        inputActive = true;
        clk = true;
        sc_start(tickHalfTime);
        ++tick;
        clk = false;
        sc_start(tickHalfTime);
        inputActive = false;
    };

    incrementTick();
    auto t1 = std::thread(&pollStdin);
    while (true)
    {
        if (consoleInputFlag.load())
        {
            insertValue(static_cast<Defs::InputValues>(consoleInputValue.load()));
            consoleInputFlag.store(false);
        }
        else
        {
            incrementTick();
        }
    }
    t1.join();
    constexpr auto insertCounter = 2U;
    auto accTest = 0U;
    bool ok = true;
    return 0;
}
