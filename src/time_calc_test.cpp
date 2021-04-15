#include <stdio.h>
#include <systemc.h>

#include "Logger.h"
#include "TimeCalcModule.h"
#include "Utils.h"

using namespace std;

int sc_main(int, char**)
{
    sc_signal<bool> clk;
    sc_signal<bool> reset;
    sc_signal<uint8_t> coin;
    sc_signal<int> timeMinutesOut;

    TimeCalcModule timeCalc("TimeCalc");
    timeCalc.clk(clk);
    timeCalc.reset(reset);
    timeCalc.coin(coin);
    timeCalc.timeMinutes(timeMinutesOut);

    clk = false;
    reset = false;

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
    sc_start(tickHalfTime);

    constexpr auto insertCounter = 2U;
    auto accTest = 0U;
    auto testSum = [&] {
        accTest += TimeCalcModule::coinToMinutes(coin.read()) * insertCounter;
        if (accTest != timeMinutesOut.read())
        {
            DefaultLogger{}.error("Invalid sum! Got %d, extected %d", timeMinutesOut.read(), accTest);
        }
    };
    auto insertCoins = [&](uint8_t coinVal) {
        coin = coinVal;
        for (auto i = 0U; i < insertCounter; ++i)
        {
            console.print("coin inserted: %s", TimeCalcModule::coinToString(coinVal));
            incrementTick();
            console.print("accTime: %d minutes.", timeMinutesOut.read());
        }
    };

    insertCoins(TimeCalcModule::none);
    testSum();
    insertCoins(TimeCalcModule::fifty_gr);
    testSum();
    insertCoins(TimeCalcModule::one_pln);
    testSum();
    insertCoins(TimeCalcModule::two_pln);
    testSum();
    insertCoins(TimeCalcModule::five_pln);
    testSum();
    return 0;
}
