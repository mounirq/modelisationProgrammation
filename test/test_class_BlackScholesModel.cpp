#include "gtest/gtest.h"
#include "../src/BlackScholesModel.hpp"

class BlackScholesModelTest: public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(BlackScholesModelTest, constructeur_sans_parametre){
    BlackScholesModel model = BlackScholesModel();
    EXPECT_EQ(0, model.size_);
    EXPECT_EQ(0, model.r_);
    EXPECT_EQ(0, model.rho_);
    PnlVect * tmp = pnl_vect_new();
    int isEqual = pnl_vect_isequal(tmp, model.spot_, 0);
    EXPECT_TRUE(isEqual);
    isEqual = pnl_vect_isequal(tmp, model.sigma_, 0);
    EXPECT_TRUE(isEqual);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
