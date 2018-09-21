#include "gtest/gtest.h"
#include "../src/BasketOption.hpp"
#include <math.h>
class BasketOptionTest: public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(BasketOptionTest, constructeur_sans_parametre){

	BasketOption optionBasket = BasketOption();

    EXPECT_EQ(0, optionBasket.T_);
    EXPECT_EQ(0, optionBasket.nbTimeSteps_);
    EXPECT_EQ(0, optionBasket.size_);
    EXPECT_EQ(0, optionBasket.strike_);
    PnlVect * tmp = pnl_vect_new();
    int isEqual = pnl_vect_isequal(tmp, optionBasket.weights_, 0);
    EXPECT_TRUE(isEqual);
}


TEST_F(BasketOptionTest, constructeur_avec_parametre){

	double T = 30;
	int nbTimeSteps = 5;
	int size = 3;
	PnlVect * weights = pnl_vect_create_from_scalar(size, 1/3);
	double strike = 12;
	BasketOption optionBasket = BasketOption(T, nbTimeSteps, size, strike,weights);

    EXPECT_EQ(30, optionBasket.T_);
    EXPECT_EQ(5, optionBasket.nbTimeSteps_);
    EXPECT_EQ(3, optionBasket.size_);
    EXPECT_EQ(12, optionBasket.strike_);
    int isEqual = pnl_vect_isequal(weights, optionBasket.weights_, 0);
    EXPECT_TRUE(isEqual);
}



TEST_F(BasketOptionTest, constructeur_par_copie){

	double T = 30;
	int nbTimeSteps = 5;
	int size = 3;
	PnlVect * weights = pnl_vect_create_from_scalar(size, 1/3);
	double strike = 12;

	// utilisation du constructeurs avec paramètres
	BasketOption optionBasket = BasketOption(T, nbTimeSteps, size,strike, weights);

	BasketOption optionBasketCopie = BasketOption(optionBasket);

    EXPECT_EQ(30, optionBasketCopie.T_);
    EXPECT_EQ(5, optionBasketCopie.nbTimeSteps_);
    EXPECT_EQ(3, optionBasketCopie.size_);
    EXPECT_EQ(12, optionBasketCopie.strike_);
    int isEqual = pnl_vect_isequal(weights, optionBasketCopie.weights_, 0);
    EXPECT_TRUE(isEqual);
}


TEST_F(BasketOptionTest, méthode_payoff){
    PnlVect * weights = pnl_vect_create_from_list(2, 0.3, 0.7);
    BasketOption basketOption = BasketOption(2, 2, 2, 12, weights);
    int nbUnderlyings = 2;
    PnlMat * past = pnl_mat_create(3, nbUnderlyings);
    pnl_mat_set(past, 0, 0, 10);
    pnl_mat_set(past, 0, 1, 15);
    pnl_mat_set(past, 1, 0, 10.292682);
    pnl_mat_set(past, 1, 1, 15.169634);
    pnl_mat_set(past, 2, 0, 10.263704);
    pnl_mat_set(past, 2, 1, 15.039295);
    double payOffReturned = basketOption.payoff(past);
    double erreur = fabs(payOffReturned-1.60662);
    bool b = (erreur <= 0.001 );
    EXPECT_TRUE(b);
}

TEST_F(BasketOptionTest, méthode_payoff2){
    PnlVect * weights = pnl_vect_create_from_list(2,0.5, 0.5);
    BasketOption optionBasket  = BasketOption(2, 4, 2, 1.5, weights);

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
    double payOffReturned = optionBasket.payoff(past);
    EXPECT_EQ(0.5, payOffReturned);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
