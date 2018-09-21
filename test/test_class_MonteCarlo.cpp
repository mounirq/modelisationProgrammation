#include "gtest/gtest.h"
#include "../src/MonteCarlo.hpp"
#include "../src/BasketOption.hpp"
#include "../src/AsianOption.hpp"
#include "../src/BlackScholesModel.hpp"
#include <math.h>

class MonteCarloTest: public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
};

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

    BlackScholesModel *bs = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);

    pnl_rng_sseed(rng, time(NULL));

    double prix1;
    double ic1;

    int nbSamples = 50000;
    PnlRandom * pnlRandom = new PnlRandom();
    MonteCarlo *mc = new MonteCarlo(bs, optionBasket, pnlRandom, 0.01, nbSamples);

    mc->price(prix1,ic1);
    double erreur = fabs( prix1 - 7.79265);
    bool b = (erreur = 0.001 );
    EXPECT_TRUE(b);
    PnlVect *delta = pnl_vect_create_from_scalar(size, 1);
    PnlMat *past = pnl_mat_create_from_scalar(size, nbTimeSteps+1, 100);
    PnlVect * icDelta = pnl_vect_create_from_scalar(size, 0);
    mc->delta(past, 0, delta, icDelta);

    pnl_vect_free(&initialSpots);
    pnl_vect_free(&volatilities);
    pnl_vect_free(&weights);
    pnl_rng_free(&rng);
    pnl_vect_free(&delta);
    pnl_mat_free(&past);

    delete mc;
    delete bs;
    delete optionBasket;
}


TEST_F(MonteCarloTest, test_price_asian){

    int size = 2;
    double strike = 100.0;
    PnlVect* initialSpots = pnl_vect_create_from_scalar(2,100);
    double maturity = 1.5;
    PnlVect* volatilities = pnl_vect_create_from_scalar(2,0.2);
    double intrestRate = 0.02;
    double rho = 0.0;
    PnlVect* weights = pnl_vect_create_from_scalar(2,0.5);
    int nbTimeSteps = 150;
    AsianOption *asianOption = new AsianOption(maturity, nbTimeSteps, size, strike, weights);
    BlackScholesModel *bs = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);
    double prix1;
    double ic1;
    int nbSamples = 50000;
    PnlRandom *rng = new PnlRandom();

    MonteCarlo *mc = new MonteCarlo(bs, asianOption, rng, 0.01, nbSamples);
    mc->price(prix1,ic1);

    double erreur = fabs( prix1 - 4.75729);
    bool b = (erreur = 0.001 );
    EXPECT_TRUE(b);

    pnl_vect_free(&initialSpots);
    pnl_vect_free(&volatilities);
    pnl_vect_free(&weights);

    delete mc;
    delete bs;
    delete asianOption;

}

TEST_F(MonteCarloTest, test_price_basket_file){

    MonteCarlo *mc = new MonteCarlo((char *) "/user/2/.base/errounda/home/3AIF/modelisationProgrammation/test/data/basket_1.dat");


    double prix1;
    double ic1;
    mc->price(prix1,ic1);

    double erreur = fabs( prix1 - 13.616294)/prix1;
    bool b = (erreur < 0.05 );
    EXPECT_TRUE(b);

    //delete mc;
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

