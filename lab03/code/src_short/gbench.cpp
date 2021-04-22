/* 26.01.16 - Xavier Ruppen - HPC - REDS - HEIG-VD */

#include <stdio.h>
#include <stdlib.h>
#include "benchmark/benchmark.h"
#include "bench.h"


extern "C" {
#include "array_util.h"
#include "list_util.h"
}

#define DATA_SIZE 100000

class short_listFixture : public benchmark::Fixture {
public :
    void SetUp(const ::benchmark::State&)
    {
        list = list_init(DATA_SIZE);
    }

    void TearDown(const ::benchmark::State&)
    {
       list_clear(list);
    }

protected :
    struct list_element *list;
   
};

class short_arrayFixture : public benchmark::Fixture {
public :
    void SetUp(const ::benchmark::State&)
    {
        array = array_init(DATA_SIZE);
    }

    void TearDown(const ::benchmark::State&)
    {
        array_clear(array);
    }

protected :
    uint64_t *array;
};



BENCHMARK_F(short_listFixture, list_sort)(benchmark::State& state) {
    while (state.KeepRunning()) {
       list_sort(list);
    }
}

BENCHMARK_F(short_arrayFixture, array_sort)(benchmark::State& state) {
    while (state.KeepRunning()) {
        array_sort(array,DATA_SIZE);
    }
}

BENCHMARK_MAIN();
