add_executable(test-parser test_parser.cpp)
target_link_libraries(test-parser jl_parser ${LIBS})

add_executable(mc-example mc_example.cpp)
target_link_libraries(mc-example jl_parser ${LIBS})


add_executable(test_basketOption test_basketOption.cpp)
target_link_libraries(test_basketOption prng ${LIBS})

add_executable(test_asianOption test_asianOption.cpp)
target_link_libraries(test_asianOption prng ${LIBS})


add_executable(test_blackScholesModel test_blackScholesModel.cpp)
target_link_libraries(test_blackScholesModel prng ${LIBS})

add_executable(mc-price-basket test_mc_price_basket.cpp)
target_link_libraries(mc-price-basket jl_parser prng ${LIBS})

add_executable(mc-price-asian test_mc_price_asian.cpp)
target_link_libraries(mc-price-asian jl_parser prng ${LIBS})

add_executable(test_class_BlackScholesModel test_class_BlackScholesModel.cpp)
target_link_libraries(test_class_BlackScholesModel prng gtest ${LIBS})


