add_executable(test-parser test_parser.cpp)
target_link_libraries(test-parser jl_parser ${LIBS})

add_executable(test_mc_delta test_mc_delta.cpp)
target_link_libraries(test_mc_delta prng ${LIBS})

add_executable(test_basketOption test_basketOption.cpp)
target_link_libraries(test_basketOption prng ${LIBS})

add_executable(test_asianOption test_asianOption.cpp)
target_link_libraries(test_asianOption prng ${LIBS})

add_executable(test_performanceOption test_performanceOption.cpp)
target_link_libraries(test_performanceOption prng ${LIBS})

add_executable(test_blackScholesModel test_blackScholesModel.cpp)
target_link_libraries(test_blackScholesModel prng ${LIBS})

add_executable(mc-price-basket test_mc_price_basket.cpp)
target_link_libraries(mc-price-basket jl_parser prng ${LIBS})

add_executable(mc-price-asian test_mc_price_asian.cpp)
target_link_libraries(mc-price-asian jl_parser prng ${LIBS})

add_executable(test_class_BlackScholesModel test_class_BlackScholesModel.cpp)
target_link_libraries(test_class_BlackScholesModel prng gtest ${LIBS})

add_executable(test_class_basketOption test_class_basketOption.cpp)
target_link_libraries(test_class_basketOption prng gtest ${LIBS})

add_executable(test_class_asianOption test_class_asianOption.cpp)
target_link_libraries(test_class_asianOption prng gtest ${LIBS})

add_executable(test_class_performanceOption test_class_performanceOption.cpp)
target_link_libraries(test_class_performanceOption prng gtest ${LIBS})