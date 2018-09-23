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

/* Test options price at t = 0 : */

TEST_F(MonteCarloTest, test_price_basket_file){
    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../../data/basket.dat");

    double basketOptionPrice;
    double ic1;
    monteCarlo->price(basketOptionPrice,ic1);

    double error = fabs( basketOptionPrice - 13.627)/13.627;
    bool b = (error < 0.05 );
    EXPECT_TRUE(b);

    delete monteCarlo;
}

TEST_F(MonteCarloTest, test_price_basket1_file){

    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../../data/basket_1.dat");

    double basketOptionPrice;
    double ic1;
    monteCarlo->price(basketOptionPrice,ic1);

    double error = fabs( basketOptionPrice - 13.616294)/13.616294;
    bool b = (error < 0.05 );
    EXPECT_TRUE(b);

    delete monteCarlo;
}


TEST_F(MonteCarloTest, test_price_asian_file){

    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../../data/asian.dat");

    double asianOptionPrice;
    double ic1;
    monteCarlo->price(asianOptionPrice,ic1);

    double erreur = fabs( asianOptionPrice - 4.75729)/4.75729;
    bool b = (erreur < 0.05 );
    EXPECT_TRUE(b);

    delete monteCarlo;
}

TEST_F(MonteCarloTest, test_price_perf_file){

    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../../data/perf.dat");

    double perfOptionPrice;
    double ic1;
    monteCarlo->price(perfOptionPrice,ic1);

    double error = fabs(perfOptionPrice - 1.257353)/1.257353;
    bool b = (error < 0.05 );
    EXPECT_TRUE(b);

    delete monteCarlo;
}

/* Test options delta at t = 0 : */
TEST_F(MonteCarloTest, test_delta_basket1_file){
    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../../data/basket_1.dat");

    int size = monteCarlo->mod_->size_;
    int nbTimeSteps = 1;
    PnlVect *delta = pnl_vect_create_from_scalar(size, 1);
    PnlMat *past = pnl_mat_create_from_scalar(size, nbTimeSteps+1, 0);
    PnlVect * icDelta = pnl_vect_create_from_scalar(size, 0);
    monteCarlo->delta(past, 0, delta, icDelta);
    PnlVect *deltaFoundedInFile = pnl_vect_create_from_list(size, 0.0248956, 0.0249324, 0.0249503,0.0250191,0.0249342,0.024961,0.0249808,0.0248986,0.0249032,0.0249421,0.0249554,0.0249416,0.0249236,0.0249541,0.0249515,0.0250078,0.0248439,0.0248783,0.0249421,0.0249178,0.0249168,0.0249511,0.0249393,0.0250007,0.0249344,0.0248973,0.0249136,0.0249047,0.024927,0.0248953,0.0249739,0.024962,0.0249467,0.0249322,0.0249571,0.0249858,0.0248853,0.024914,0.024861,0.0250143);
    /* Test if the relative error is < 1% */
    PnlVect * relativeError = pnl_vect_copy(delta);
    pnl_vect_minus_vect(relativeError, deltaFoundedInFile);
    double error = pnl_vect_norm_two(relativeError)/pnl_vect_norm_two(deltaFoundedInFile);
    bool smallError = (error < 0.01);
    EXPECT_TRUE(smallError);
    delete monteCarlo;
    pnl_vect_free(&delta);
    pnl_mat_free(&past);
    pnl_vect_free(&icDelta);
    pnl_vect_free(&deltaFoundedInFile);
    pnl_vect_free(&relativeError);
}

TEST_F(MonteCarloTest, test_delta_basket2_file){

    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../../data/basket_2.dat");

    int size = monteCarlo->mod_->size_;
    int nbTimeSteps = 1;
    PnlVect *delta = pnl_vect_create_from_scalar(size, 1);
    PnlMat *past = pnl_mat_create_from_scalar(size, nbTimeSteps+1, 0);
    PnlVect * icDelta = pnl_vect_create_from_scalar(size, 0);
    monteCarlo->delta(past, 0, delta, icDelta);
    PnlVect *deltaFoundedInFile = pnl_vect_create_from_list(size, 0.0162094,0.01621,0.01622,0.0162203,0.0162145,0.0162044,0.016215,0.0162163,0.0161995,0.0161993,0.0162085,0.0161999,0.0161848,0.0161824,0.0162018,0.0161994,0.0161971,0.0162066,0.0162119,0.0162086,0.0162088,0.0162014,0.0162149,0.0162236,0.0162311,0.0162138,0.0162064,0.0162129,0.0162166,0.0162183,0.0161925,0.0162276,0.016189,0.0161958,0.0162017,0.0162142,0.0162028,0.0162092,0.0162104,0.0162025);
    /* Test if the relatives errors of all the element are < 5% */
    PnlVect * relativeError = pnl_vect_copy(delta);
    pnl_vect_minus_vect(relativeError, deltaFoundedInFile);
    double error = pnl_vect_norm_two(relativeError)/pnl_vect_norm_two(deltaFoundedInFile);
    bool smallError = (error < 0.01);
    EXPECT_TRUE(smallError);
    delete monteCarlo;
    pnl_vect_free(&delta);
    pnl_mat_free(&past);
    pnl_vect_free(&icDelta);
    pnl_vect_free(&deltaFoundedInFile);
    pnl_vect_free(&relativeError);
}


TEST_F(MonteCarloTest, test_delta_asian_file){
    MonteCarlo *monteCarlo = new MonteCarlo((char *) "../../data/asian.dat");
    int size = monteCarlo->mod_->size_;
    int nbTimeSteps = 1;
    PnlVect *delta = pnl_vect_create_from_scalar(size, 1);
    PnlMat *past = pnl_mat_create_from_scalar(size, nbTimeSteps+1, 0);
    PnlVect * icDelta = pnl_vect_create_from_scalar(size, 0);
    monteCarlo->delta(past, 0, delta, icDelta);
    PnlVect *deltaFoundedInFile = pnl_vect_create_from_list(size, 0.281640, 0.281951);
    /* Test if the relatives errors of all the element are < 5% */
    PnlVect * relativeError = pnl_vect_copy(delta);
    pnl_vect_minus_vect(relativeError, deltaFoundedInFile);
    double error = pnl_vect_norm_two(relativeError)/pnl_vect_norm_two(deltaFoundedInFile);
    bool smallError = (error < 0.01);
    EXPECT_TRUE(smallError);
    delete monteCarlo;
    pnl_vect_free(&delta);
    pnl_mat_free(&past);
    pnl_vect_free(&icDelta);
    pnl_vect_free(&deltaFoundedInFile);
    pnl_vect_free(&relativeError);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

