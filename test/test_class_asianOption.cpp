#include "gtest/gtest.h"
#include "../src/AsianOption.hpp"
#include <math.h>
class AsianOptionTest: public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(AsianOptionTest, constructeur_sans_parametre){

	AsianOption optionAsian = AsianOption();

    EXPECT_EQ(0, optionAsian.T_);
    EXPECT_EQ(0, optionAsian.nbTimeSteps_);
    EXPECT_EQ(0, optionAsian.size_);
    EXPECT_EQ(0, optionAsian.strike_);
    PnlVect * tmp = pnl_vect_new();
    int isEqual = pnl_vect_isequal(tmp, optionAsian.weights_, 0);
    EXPECT_TRUE(isEqual);
}


TEST_F(AsianOptionTest, constructeur_avec_parametre){

	double T = 30;
	int nbTimeSteps = 5;
	int size = 3;
	PnlVect * weights = pnl_vect_create_from_scalar(size, 1/3);
	double strike = 12;
	AsianOption optionAsian = AsianOption(T, nbTimeSteps, size, strike,weights);

    EXPECT_EQ(30, optionAsian.T_);
    EXPECT_EQ(5, optionAsian.nbTimeSteps_);
    EXPECT_EQ(3, optionAsian.size_);
    EXPECT_EQ(12, optionAsian.strike_);
    int isEqual = pnl_vect_isequal(weights, optionAsian.weights_, 0);
    EXPECT_TRUE(isEqual);
}



TEST_F(AsianOptionTest, constructeur_par_copie){

	double T = 30;
	int nbTimeSteps = 5;
	int size = 3;
	PnlVect * weights = pnl_vect_create_from_scalar(size, 1/3);
	double strike = 12;

	// utilisation du constructeurs avec paramètres
	AsianOption optionAsian = AsianOption(T, nbTimeSteps, size,strike, weights);

	AsianOption optionAsianCopie = AsianOption(optionAsian);

    EXPECT_EQ(30, optionAsianCopie.T_);
    EXPECT_EQ(5, optionAsianCopie.nbTimeSteps_);
    EXPECT_EQ(3, optionAsianCopie.size_);
    EXPECT_EQ(12, optionAsianCopie.strike_);
    int isEqual = pnl_vect_isequal(weights, optionAsianCopie.weights_, 0);
    EXPECT_TRUE(isEqual);
}


TEST_F(AsianOptionTest, méthode_payoff){
	PnlVect * weights = pnl_vect_create_from_list(3, 0.3, 0.2, 0.5);
	AsianOption asianOption = AsianOption(4,2,3,10,weights);
	int nbUnderlyings = 3;
	PnlMat * past = pnl_mat_create(3, nbUnderlyings);
	pnl_mat_set(past, 0, 0, 10);
	pnl_mat_set(past, 0, 1, 11);
	pnl_mat_set(past, 0, 2, 12);
	pnl_mat_set(past, 1, 0, 9);
	pnl_mat_set(past, 1, 1, 10);
	pnl_mat_set(past, 1, 2, 11);
	pnl_mat_set(past, 2, 0, 11);
	pnl_mat_set(past, 2, 1, 9);
	pnl_mat_set(past, 2, 2, 10);
	double payOffReturned = asianOption.payoff(past);
	EXPECT_EQ(0.5, payOffReturned);
}


TEST_F(AsianOptionTest, méthode_payoff2){
    PnlVect * weights = pnl_vect_create_from_list(2,0.5, 0.5);
    AsianOption optionAsian  = AsianOption(2, 4, 2, 1, weights);

    int nbUnderlyings = 2;
    PnlMat * past = pnl_mat_create(5, nbUnderlyings);
    pnl_mat_set(past, 0, 0, 2);
    pnl_mat_set(past, 0, 1, 2);
    for(int i = 1; i<4; ++i)
    {
        for (int j =0; j<2; ++j)
        {
            pnl_mat_set(past, i, j, 1);
        }
    }
    pnl_mat_set(past, 4, 0, 2);
    pnl_mat_set(past, 4, 1, 2);
    double payOffReturned = optionAsian.payoff(past);
    double erreur = fabs(payOffReturned-0.4);
    bool b = (erreur <= 0.1 );
    EXPECT_TRUE(b);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
