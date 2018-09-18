cmake_minimum_required(VERSION 3.0)

set(SOURCES BlackScholesModel.cpp MonteCarlo.cpp Option.cpp BasketOption.cpp AsianOption.cpp)

add_library(prng ${SOURCES})
