add_executable(test_blackScholesModel test_blackScholesModel.cpp)
target_link_libraries(test_blackScholesModel prng ${LIBS})

add_executable(mc-price-basket test_mc_price_basket.cpp)
target_link_libraries(mc-price-basket jl_parser prng ${LIBS})

add_executable(mc-price-asian test_mc_price_asian.cpp)
target_link_libraries(mc-price-asian jl_parser prng ${LIBS})
