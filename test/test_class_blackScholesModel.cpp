#include "gtest/gtest.h"
#include "../src/BlackScholesModel.hpp"
#include "../src/FakeRandom.hpp"
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

TEST_F(BlackScholesModelTest, asset_in_t_equals_0){
    int nbTimeSteps = 5;
    int nbUnderlyings = 2;
    double maturity = 1;
    PnlVect * sigma = pnl_vect_create(nbUnderlyings);
    PnlVect * spot = pnl_vect_create(nbUnderlyings);
    pnl_vect_set(sigma, 0, 0.25);
    pnl_vect_set(sigma, 1, 0.20);
    pnl_vect_set(spot, 0, 10);
    pnl_vect_set(spot, 1, 15);
    BlackScholesModel model = BlackScholesModel(nbUnderlyings, 0.03, 0.15, sigma, spot);
    PnlMat * path = pnl_mat_create(nbTimeSteps + 1, nbUnderlyings);
    FakeRandom * fakeRandom = new FakeRandom();
    model.asset(path, maturity, nbTimeSteps, fakeRandom);
    PnlMat * corrMat = pnl_mat_create_from_scalar(nbUnderlyings, nbUnderlyings, 0.15);
    for(int i=0; i<nbUnderlyings; i++)
    {
        pnl_mat_set(corrMat, i, i, 1);
    }
    pnl_mat_chol(corrMat);
    double L_1G = pnl_mat_get(corrMat,0,0) +pnl_mat_get(corrMat,0,1) ;
    double L_2G = pnl_mat_get(corrMat,1,0) + pnl_mat_get(corrMat,1,1) ;
    PnlVect *lVector = pnl_vect_create_from_list(2, L_1G, L_2G);
    PnlMat *matrixSpot = pnl_mat_create(6,2);
    pnl_mat_set(matrixSpot, 0, 0, 10);
    pnl_mat_set(matrixSpot, 0, 1, 15);
    double tmp;
    for(int i = 1; i < 6; i++){
        for(int j = 0; j < 2; j++){
            tmp = pnl_mat_get(matrixSpot,i-1,j)*exp((0.03 - (pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j))/2)*0.2 + pnl_vect_get(sigma,j)* sqrt(0.2) * pnl_vect_get(lVector,j));
            pnl_mat_set(matrixSpot, i, j,tmp);
        }
    }
    EXPECT_TRUE(pnl_mat_isequal(matrixSpot, path, 1e-15));

    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_mat_free(&path);
    delete fakeRandom;
    pnl_mat_free(&corrMat);
    pnl_vect_free(&lVector);
    pnl_mat_free(&matrixSpot);
}

TEST_F(BlackScholesModelTest, asset_in_t_multiple_of_discretization_path){
    int nbTimeSteps = 5;
    int nbUnderlyings = 2;
    double maturity = 1;
    PnlVect * sigma = pnl_vect_create(nbUnderlyings);
    PnlVect * spot = pnl_vect_create(nbUnderlyings);
    pnl_vect_set(sigma, 0, 0.25);
    pnl_vect_set(sigma, 1, 0.20);
    pnl_vect_set(spot, 0, 10);
    pnl_vect_set(spot, 1, 15);
    BlackScholesModel model = BlackScholesModel(nbUnderlyings, 0.03, 0.15, sigma, spot);
    PnlMat * past = pnl_mat_create(3, nbUnderlyings);

    pnl_mat_set(past, 0, 0, 10);
    pnl_mat_set(past, 0, 1, 15);
    pnl_mat_set(past, 1, 0, 11);
    pnl_mat_set(past, 1, 1, 12);
    pnl_mat_set(past, 2, 0, 13);
    pnl_mat_set(past, 2, 1, 14);
    PnlMat * path = pnl_mat_create(nbTimeSteps + 1, nbUnderlyings);
    FakeRandom * fakeRandom = new FakeRandom();
    model.asset(path, 0.4, maturity, nbTimeSteps, fakeRandom, past);
    PnlMat * corrMat = pnl_mat_create_from_scalar(nbUnderlyings, nbUnderlyings, 0.15);
    for(int i=0; i<nbUnderlyings; i++)
    {
        pnl_mat_set(corrMat, i, i, 1);
    }
    pnl_mat_chol(corrMat);
    double L_1G = pnl_mat_get(corrMat,0,0) +pnl_mat_get(corrMat,0,1) ;
    double L_2G = pnl_mat_get(corrMat,1,0) + pnl_mat_get(corrMat,1,1) ;
    PnlVect *lVector = pnl_vect_create_from_list(2, L_1G, L_2G);
    PnlMat *matrixSpot = pnl_mat_create(6,2);
    pnl_mat_set(matrixSpot, 0, 0, 10);
    pnl_mat_set(matrixSpot, 0, 1, 15);
    pnl_mat_set(matrixSpot, 1, 0, 11);
    pnl_mat_set(matrixSpot, 1, 1, 12);
    pnl_mat_set(matrixSpot, 2, 0, 13);
    pnl_mat_set(matrixSpot, 2, 1, 14);
    double tmp;
    for(int i = 3; i < 6; i++){
        for(int j = 0; j < 2; j++){
            tmp = pnl_mat_get(matrixSpot,i-1,j)*exp((0.03 - (pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j))/2)*0.2 + pnl_vect_get(sigma,j)* sqrt(0.2) * pnl_vect_get(lVector,j));
            pnl_mat_set(matrixSpot, i, j,tmp);
        }
    }
    EXPECT_TRUE(pnl_mat_isequal(matrixSpot, path, 1e-15));

    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_mat_free(&path);
    delete fakeRandom;
    pnl_mat_free(&corrMat);
    pnl_vect_free(&lVector);
    pnl_mat_free(&matrixSpot);
    pnl_mat_free(&past);
}

TEST_F(BlackScholesModelTest, asset_in_t_not_multiple_of_discretization_path){
    int nbTimeSteps = 5;
    int nbUnderlyings = 2;
    double maturity = 1;
    PnlVect * sigma = pnl_vect_create(nbUnderlyings);
    PnlVect * spot = pnl_vect_create(nbUnderlyings);
    pnl_vect_set(sigma, 0, 0.25);
    pnl_vect_set(sigma, 1, 0.20);
    pnl_vect_set(spot, 0, 10);
    pnl_vect_set(spot, 1, 15);
    BlackScholesModel model = BlackScholesModel(nbUnderlyings, 0.03, 0.15, sigma, spot);
    PnlMat * past = pnl_mat_create(4, nbUnderlyings);
    pnl_mat_set(past, 0, 0, 10);
    pnl_mat_set(past, 0, 1, 15);
    pnl_mat_set(past, 1, 0, 11);
    pnl_mat_set(past, 1, 1, 12);
    pnl_mat_set(past, 2, 0, 13);
    pnl_mat_set(past, 2, 1, 14);
    pnl_mat_set(past, 3, 0, 11.5);
    pnl_mat_set(past, 3, 1, 12.5);
    PnlMat * path = pnl_mat_create(nbTimeSteps + 1, nbUnderlyings);
    FakeRandom * fakeRandom = new FakeRandom();
    model.asset(path, 0.5, maturity, nbTimeSteps, fakeRandom, past);
    PnlMat * corrMat = pnl_mat_create_from_scalar(nbUnderlyings, nbUnderlyings, 0.15);
    for(int i=0; i<nbUnderlyings; i++)
    {
        pnl_mat_set(corrMat, i, i, 1);
    }
    pnl_mat_chol(corrMat);
    double L_1G = pnl_mat_get(corrMat,0,0) +pnl_mat_get(corrMat,0,1) ;
    double L_2G = pnl_mat_get(corrMat,1,0) + pnl_mat_get(corrMat,1,1) ;
    PnlVect *lVector = pnl_vect_create_from_list(2, L_1G, L_2G);
    PnlMat *matrixSpot = pnl_mat_create(6,2);
    pnl_mat_set(matrixSpot, 0, 0, 10);
    pnl_mat_set(matrixSpot, 0, 1, 15);
    pnl_mat_set(matrixSpot, 1, 0, 11);
    pnl_mat_set(matrixSpot, 1, 1, 12);
    pnl_mat_set(matrixSpot, 2, 0, 13);
    pnl_mat_set(matrixSpot, 2, 1, 14);
    pnl_mat_set(matrixSpot, 3, 0, 11.5);
    pnl_mat_set(matrixSpot, 3, 1, 12.5);
    double tmp;
    double differenceTime = 0;
    for(int i = 3; i < 6; i++){
        for(int j = 0; j < 2; j++){
            if(i == 3){
                differenceTime = i*0.2 - 0.5;
                tmp = pnl_mat_get(matrixSpot,i,j)*exp((0.03 - (pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j))/2)*differenceTime +
                                                      pnl_vect_get(sigma,j)* sqrt(differenceTime) * pnl_vect_get(lVector,j));
            }
            else{
                differenceTime = 0.2;
                tmp = pnl_mat_get(matrixSpot,i-1,j)*exp((0.03 - (pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j))/2)*differenceTime +
                                                      pnl_vect_get(sigma,j)* sqrt(differenceTime) * pnl_vect_get(lVector,j));
            }
            pnl_mat_set(matrixSpot, i, j,tmp);
        }
    }
    EXPECT_TRUE(pnl_mat_isequal(matrixSpot, path, 1e-15));

    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_mat_free(&path);
    delete fakeRandom;
    pnl_mat_free(&corrMat);
    pnl_vect_free(&lVector);
    pnl_mat_free(&matrixSpot);
    pnl_mat_free(&past);
}

TEST_F(BlackScholesModelTest, asset_in_t_equals_maturity){
    int nbTimeSteps = 5;
    int nbUnderlyings = 2;
    double maturity = 1;
    PnlVect * sigma = pnl_vect_create(nbUnderlyings);
    PnlVect * spot = pnl_vect_create(nbUnderlyings);
    pnl_vect_set(sigma, 0, 0.25);
    pnl_vect_set(sigma, 1, 0.20);
    pnl_vect_set(spot, 0, 10);
    pnl_vect_set(spot, 1, 15);
    BlackScholesModel model = BlackScholesModel(nbUnderlyings, 0.03, 0.15, sigma, spot);
    PnlMat * past = pnl_mat_create(6, nbUnderlyings);
    pnl_mat_set(past, 0, 0, 10);
    pnl_mat_set(past, 0, 1, 15);
    pnl_mat_set(past, 1, 0, 11);
    pnl_mat_set(past, 1, 1, 12);
    pnl_mat_set(past, 2, 0, 13);
    pnl_mat_set(past, 2, 1, 14);
    pnl_mat_set(past, 3, 0, 13);
    pnl_mat_set(past, 3, 1, 16);
    pnl_mat_set(past, 4, 0, 17);
    pnl_mat_set(past, 4, 1, 11);
    PnlMat * path = pnl_mat_create(nbTimeSteps + 1, nbUnderlyings);
    FakeRandom * fakeRandom = new FakeRandom();
    model.asset(path, 1, maturity, nbTimeSteps, fakeRandom, past);

    EXPECT_TRUE(pnl_mat_isequal(path, past, 1e-15));

    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    pnl_mat_free(&path);
    delete fakeRandom;
    pnl_mat_free(&past);
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
