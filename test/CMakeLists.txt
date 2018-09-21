add_executable(test-parser test_parser.cpp)
target_link_libraries(test-parser jl_parser ${LIBS})

add_executable(mc-example mc_example.cpp test_mc_basket_from_file.cpp)
target_link_libraries(mc-example jl_parser ${LIBS})

add_executable(test_mc_delta test_mc_delta.cpp)
target_link_libraries(test_mc_delta prng ${LIBS})

add_executable(mc-price-basket test_mc_price_basket.cpp)
target_link_libraries(mc-price-basket jl_parser prng ${LIBS})

add_executable(mc-price-asian test_mc_price_asian.cpp)
target_link_libraries(mc-price-asian jl_parser prng ${LIBS})

add_executable(test_class_blackScholesModel test_class_blackScholesModel.cpp)
target_link_libraries(test_class_blackScholesModel prng gtest ${LIBS})

add_executable(test_class_basketOption test_class_basketOption.cpp)
target_link_libraries(test_class_basketOption prng gtest ${LIBS})

add_executable(test_class_asianOption test_class_asianOption.cpp)
target_link_libraries(test_class_asianOption prng gtest ${LIBS})

add_executable(test_class_performanceOption test_class_performanceOption.cpp)
target_link_libraries(test_class_performanceOption prng gtest ${LIBS})

add_executable(test_pl_basket test_PL_basket.cpp)
target_link_libraries(test_pl_basket prng ${LIBS})

add_executable(test_class_FakeRandom test_class_FakeRandom.cpp)
target_link_libraries(test_class_FakeRandom prng gtest ${LIBS})