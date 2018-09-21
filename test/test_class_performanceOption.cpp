#include "gtest/gtest.h"
#include "../src/PerformanceOption.hpp"
#include <math.h>

class PerformanceOptionTest: public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

TEST_F(PerformanceOptionTest, constructeur_sans_parametre){

	PerformanceOption optionPerformance = PerformanceOption();

    EXPECT_EQ(0, optionPerformance.T_);
    EXPECT_EQ(0, optionPerformance.nbTimeSteps_);
    EXPECT_EQ(0, optionPerformance.size_);
    PnlVect * tmp = pnl_vect_new();
    int isEqual = pnl_vect_isequal(tmp, optionPerformance.weights_, 0);
    EXPECT_TRUE(isEqual);
}


TEST_F(PerformanceOptionTest, constructeur_avec_parametre){

	double T = 30;
	int nbTimeSteps = 5;
	int size = 3;
	PnlVect * weights = pnl_vect_create_from_scalar(size, 1/3);
	PerformanceOption optionPerformance = PerformanceOption(T, nbTimeSteps, size, weights);

    EXPECT_EQ(30, optionPerformance.T_);
    EXPECT_EQ(5, optionPerformance.nbTimeSteps_);
    EXPECT_EQ(3, optionPerformance.size_);
    int isEqual = pnl_vect_isequal(weights, optionPerformance.weights_, 0);
    EXPECT_TRUE(isEqual);
}



TEST_F(PerformanceOptionTest, constructeur_par_copie){

	double T = 30;
	int nbTimeSteps = 5;
	int size = 3;
	PnlVect * weights = pnl_vect_create_from_scalar(size, 1/3);

	// utilisation du constructeurs avec paramètres
	PerformanceOption optionPerformance = PerformanceOption(T, nbTimeSteps, size, weights);

	PerformanceOption optionPerformanceCopie = PerformanceOption(optionPerformance);

    EXPECT_EQ(30, optionPerformanceCopie.T_);
    EXPECT_EQ(5, optionPerformanceCopie.nbTimeSteps_);
    EXPECT_EQ(3, optionPerformanceCopie.size_);
    int isEqual = pnl_vect_isequal(weights, optionPerformanceCopie.weights_, 0);
    EXPECT_TRUE(isEqual);
}


TEST_F(PerformanceOptionTest, méthode_payoff){
    PnlVect * weights = pnl_vect_create_from_list(2,0.5, 0.5);
    PerformanceOption optionPerformance  = PerformanceOption(2, 4, 2, weights);
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

    double payOffReturned = optionPerformance.payoff(past);
    EXPECT_EQ(2, payOffReturned);
}


TEST_F(PerformanceOptionTest, méthode_payoff2){
	PnlVect * weights = pnl_vect_create_from_list(3, 0.25, 0.25, 0.5);
	PerformanceOption optionPerformance = PerformanceOption(4,2,3,weights);
	int nbUnderlyings = 3;
	PnlMat * past = pnl_mat_create(3, nbUnderlyings);
	pnl_mat_set(past, 0, 0, 10);
	pnl_mat_set(past, 0, 1, 12);
	pnl_mat_set(past, 0, 2, 14);
	pnl_mat_set(past, 1, 0, 8);
	pnl_mat_set(past, 1, 1, 10);
	pnl_mat_set(past, 1, 2, 4);
	pnl_mat_set(past, 2, 0, 10);
	pnl_mat_set(past, 2, 1, 12);
	pnl_mat_set(past, 2, 2, 20);
	double payOffReturned = optionPerformance.payoff(past);
    double erreur = fabs(payOffReturned-2.38462);
    bool b = (erreur <= 0.001 );
    EXPECT_TRUE(b);

}



int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
