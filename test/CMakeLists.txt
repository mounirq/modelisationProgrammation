add_executable(test_blackScholesModel test_blackScholesModel.cpp)
target_link_libraries(test_blackScholesModel prng ${LIBS})

add_executable(mc-price-0 test_mc.cpp)
target_link_libraries(mc-price-0 jl_parser ${LIBS})
