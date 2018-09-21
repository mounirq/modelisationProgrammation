#include "gtest/gtest.h"
#include "../src/BlackScholesModel.hpp"
#include <math.h>

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

TEST_F(BlackScholesModelTest, constructeur_avec_parametre){
	int size = 5;

	// vector of volatilities set to 0.2
	PnlVect *sigma = pnl_vect_create_from_scalar(size, 0.2);

	//vector of initial spots set to 5.0
	PnlVect *spots =  pnl_vect_create_from_scalar(size, 5.0);

    BlackScholesModel model = BlackScholesModel(size, 0.1, 0.3, sigma, spots);
    EXPECT_EQ(5, model.size_);
    EXPECT_EQ(0.1, model.r_);
    EXPECT_EQ(0.3, model.rho_);
    int isEqual = pnl_vect_isequal(sigma, model.sigma_, 0);
    EXPECT_TRUE(isEqual);
    isEqual = pnl_vect_isequal(spots, model.spot_, 0);
    EXPECT_TRUE(isEqual);
}

TEST_F(BlackScholesModelTest, constructeur_par_copie){
	int size = 5;
	// vector of volatilities set to 0.2
	PnlVect *sigma = pnl_vect_create_from_scalar(size, 0.2);
	//vector of initial spots set to 5.0
	PnlVect *spots =  pnl_vect_create_from_scalar(size, 5.0);

	// constructor with parameters
    BlackScholesModel model = BlackScholesModel(size, 0.1, 0.3, sigma, spots);


    BlackScholesModel modelCopie = BlackScholesModel(model);

    EXPECT_EQ(5, modelCopie.size_);
    EXPECT_EQ(0.1, modelCopie.r_);
    EXPECT_EQ(0.3, modelCopie.rho_);
    int isEqual = pnl_vect_isequal(sigma, modelCopie.sigma_, 0);
    EXPECT_TRUE(isEqual);
    isEqual = pnl_vect_isequal(spots, modelCopie.spot_, 0);
    EXPECT_TRUE(isEqual);
}

TEST_F(BlackScholesModelTest, shift_asset){
	int size = 6;
	// vector of volatilities set to 0.2
	PnlVect *sigma = pnl_vect_create_from_scalar(size, 0.2);
	//vector of initial spots set to 5.0
	PnlVect *spots =  pnl_vect_create_from_scalar(size, 5.0);

	// constructor with parameters
    BlackScholesModel model = BlackScholesModel(size, 0.1, 0.3, sigma, spots);

    PnlMat *path = pnl_mat_create_from_scalar(10, 6, 1);
    PnlMat *shiftPath = pnl_mat_create_from_scalar(10,6,0);

	int d =2;
	double h = 1;
    double t = 3.5;
    double timeStep = 1;
	//shift_path contient la trajectoire path* dont la composante d a été shiftée par (1+h)
	model.shiftAsset(shiftPath, path, d, h, t, timeStep);
	double erreur=0;
	bool b;
	for (int i = 4; i<10; ++i)
	{
		erreur = fabs( pnl_mat_get(shiftPath, i, d)- (pnl_mat_get(path, i, d)) *(1+h) );
		b = (erreur < 0.001 );
		EXPECT_TRUE(b);
	}

}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
