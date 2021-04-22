/* 27.03.17 - Xavier Ruppen - HPC - REDS - HEIG-VD */

#include <stdio.h>
#include <stdlib.h>
#include "benchmark/benchmark.h"

extern "C" {
#include "cache.h"
}

class DataFixture : public benchmark::Fixture {
public :
    void SetUp(const ::benchmark::State&)
    {
        data = data_init(DATA_COUNT);
    }

    void TearDown(const ::benchmark::State&)
    {
        data_free(data);
    }

protected :
    struct data_t *data;
    uint8_t average, max;
};

BENCHMARK_F(DataFixture, data)(benchmark::State& state) {
    
    while (state.KeepRunning()) {
        average = data_average(data);
        max = data_max(data);
    }

}

BENCHMARK_MAIN();
