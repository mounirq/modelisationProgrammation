cmake_minimum_required(VERSION 3.0)

set(SOURCES BlackScholesModel.cpp MonteCarlo.cpp Option.cpp BasketOption.cpp AsianOption.cpp PerformanceOption.cpp RandomGenerator.cpp RandomGenerator.hpp PnlRandom.cpp PnlRandom.hpp FakeRandom.cpp FakeRandom.hpp)

add_library(prng ${SOURCES})
