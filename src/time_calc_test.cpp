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
    sc_signal<int> accumulatedGr;

    TimeCalcModule timeCalc("TimeCalc");
    timeCalc.clk(clk);
    timeCalc.reset(reset);
    timeCalc.coin(coin);
    timeCalc.timeMinutes(timeMinutesOut);
    timeCalc.accumulatedGr(accumulatedGr);

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

    bool ok = true;
    auto testSum = [&](unsigned int minutes) {
        if (minutes != timeMinutesOut.read())
        {
            ok = false;
            DefaultLogger{}.error("Invalid sum! Got %d, extected %d", timeMinutesOut.read(), minutes);
        }
    };
    auto insertCoins = [&](uint8_t coinVal, unsigned int insertCounter) {
        coin = coinVal;
        for (auto i = 0U; i < insertCounter; ++i)
        {
            console.print("coin inserted: %s (x%d)", Defs::coinToString(coinVal), insertCounter);
            incrementTick();
            console.print("accTime: %d minutes.", timeMinutesOut.read());
            console.print("total paid: %d.%d pln", accumulatedGr.read() / 100, accumulatedGr.read() % 100);
        }
    };

    insertCoins(Defs::none, 2);
    testSum(0);
    insertCoins(Defs::fifty_gr, 2);
    testSum(30);
    insertCoins(Defs::one_pln, 3);
    testSum(120);
    insertCoins(Defs::two_pln, 1);
    testSum(210);
    insertCoins(Defs::five_pln, 4);
    testSum(930);
    reset = true;
    incrementTick();
    console.print("reset");
    reset = false;
    insertCoins(Defs::five_pln, 2);
    testSum(360);
    insertCoins(Defs::two_pln, 1);
    testSum(420);
    insertCoins(Defs::one_pln, 5);
    testSum(600);
    insertCoins(Defs::fifty_gr, 1);
    testSum(615);
    insertCoins(Defs::none, 3);
    testSum(615);
    insertCoins(Defs::five_pln, 2);
    testSum(975);
    insertCoins(Defs::five_pln, 1);
    testSum(1155);
    if (ok)
    {
        DefaultLogger{}.info("Tests ok!");
    }
    else
    {

        DefaultLogger{}.error("Tests ok!");
    }
    return 0;
}
