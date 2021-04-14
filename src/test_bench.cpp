//============================================================================
// Name        : SystemCTest.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <systemc.h>

#include "TimeCalcModule.h"
#include "config.h"
#include "stdio.h"

using namespace std;

class ConsoleOut
{
  public:
    ConsoleOut(size_t &tick) : tick(tick)
    {
    }
    template <typename... Args> void print(const char *pattern, Args &&...args)
    {
        printf("t - %d: ", tick);
        printf(pattern, args...);
        puts("");
    }

  private:
    size_t &tick;
};

int sc_main(int, char **)
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
    coin = 1;

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

    for (auto i = 0U; i < 10; ++i)
    {
        incrementTick();
        console.print("accTime: %d", timeMinutesOut.read());
    }

    return 0;
}
