#include "gtest/gtest.h"
#include "../src/FakeRandom.hpp"
#include <math.h>

class FakeRandomTest: public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(FakeRandomTest, méthode_fill_matrix){

	FakeRandom *fakeRandom = new FakeRandom();

	int nbRows = 7;
	int nbCols = 5;
	PnlMat *matrix = pnl_mat_create_from_scalar(nbRows, nbCols, 0);
	fakeRandom->fillMatrix(matrix, nbRows, nbCols);
	for (int i =0; i< nbRows; ++i)
	{
		for (int j =0; j< nbCols; ++j){
			double erreur = fabs( pnl_mat_get(matrix, i, j)- 1);
			bool b = (erreur = 0.001 );
			EXPECT_TRUE(b);
		}
	}
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
