cmake_minimum_required(VERSION 3.0)

set(SOURCES AbstractPricer.cpp BlackScholesModel.cpp MonteCarlo.cpp Option.cpp BasketOption.cpp AsianOption.cpp PerformanceOption.cpp RandomGenerator.cpp PnlRandom.cpp FakeRandom.cpp Couverture.cpp Couverture.hpp PricerMC.cpp PricerMC.hpp PricerBS.cpp PricerBS.hpp)

add_subdirectory(3rdparty/jlparser)

add_library(prng ${SOURCES})
target_link_libraries(prng ${LIBS} jl_parser)