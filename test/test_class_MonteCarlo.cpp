#include "gtest/gtest.h"
#include "../src/MonteCarlo.hpp"
#include "../src/BasketOption.hpp"
#include "../src/AsianOption.hpp"
#include "../src/BlackScholesModel.hpp"
#include <math.h>

#include <unistd.h>
#define GetCurrentDir getcwd
#include<iostream>

class MonteCarloTest: public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};


std::string GetCurrentWorkingDir( void ) {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}

TEST_F(MonteCarloTest, test_price_basket){
    int size = 40;
    double strike = 100.0;
    PnlVect* initialSpots = pnl_vect_create_from_scalar(40,100);
    double maturity = 3;
    PnlVect* volatilities = pnl_vect_create_from_scalar(40,0.2);
    double intrestRate = 0.04879;
    double rho = 0.0;
    PnlVect* weights = pnl_vect_create_from_scalar(40,0.025);
    int nbTimeSteps = 1;
    BasketOption *optionBasket = new BasketOption(maturity, nbTimeSteps, size, strike, weights);

    BlackScholesModel *blackScholesModel = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);

    double prix1;
    double ic1;

    int nbSamples = 50000;
    PnlRandom * pnlRandom = new PnlRandom();
    MonteCarlo *monteCarlo = new MonteCarlo(blackScholesModel, optionBasket, pnlRandom, 0.01, nbSamples);

    monteCarlo->price(prix1,ic1);
    double erreur = fabs( (prix1 - 13.627)/13.627);
    bool b = (erreur < 0.1 );
    EXPECT_TRUE(b);
    pnl_vect_free(&initialSpots);
    pnl_vect_free(&volatilities);
    pnl_vect_free(&weights);

    delete monteCarlo;

}


TEST_F(MonteCarloTest, test_price_basket_from_file){

    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../data/basket_1.dat");

    double prix1;
    double ic1;
    monteCarlo->price(prix1,ic1);

    double erreur = fabs( prix1 - 13.616294)/13.616294;
    bool b = (erreur < 0.05 );
    EXPECT_TRUE(b);

    delete monteCarlo;
}

TEST_F(MonteCarloTest, test_price_asian_from_file){

    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../data/asian.dat");

    double prix1;
    double ic1;
    monteCarlo->price(prix1,ic1);

    double erreur = fabs( prix1 - 4.67)/4.67;
    bool b = (erreur < 0.05 );
    EXPECT_TRUE(b);

    delete monteCarlo;
}

TEST_F(MonteCarloTest, test_price_performance_from_file){

    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../data/perf.dat");

    double prix1;
    double ic1;
    monteCarlo->price(prix1,ic1);

    double erreur = fabs( prix1 - 1.257353)/1.257353;
    bool b = (erreur < 0.05 );
    EXPECT_TRUE(b);

    delete monteCarlo;
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

