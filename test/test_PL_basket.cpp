#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "../src/BasketOption.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/MonteCarlo.hpp"
#include "../src/PricerMC.hpp"
#include "../src/Couverture.hpp"

using namespace std;

int main(int argc, char **argv) {
//    int size = 5;
//    double strike = 100.0;
//    PnlVect *initialSpots = pnl_vect_create_from_scalar(5, 100);
//    double maturity = 1;
//    PnlVect *volatilities = pnl_vect_create_from_scalar(5, 0.2);
//    double intrestRate = 0.04879;
//    double rho = 0.0;
//    PnlVect *weights = pnl_vect_create_from_scalar(5, 0.2);
//    int nbTimeSteps = 1;
//
//    BasketOption *optionBasket = new BasketOption(maturity, nbTimeSteps, size, strike, weights);
//
//    BlackScholesModel *bs = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);
//
//    int nbSamples = 5000;
//
    PnlMat *market_trajectory = pnl_mat_create_from_file((char *)"../../data/market-data/simul_basket.dat");
//
//    PnlRandom * rng = new PnlRandom();

    PricerMC *pricer = new PricerMC("../../data/basket5.dat");
//    PricerMC *pricer = new PricerMC(bs, optionBasket, rng, 0.01, nbSamples);

    Couverture *couverture = new Couverture(pricer);

//    Couverture *couverture = new Couverture("../../data/basket5.dat", pricer);



    double p_and_l, pl_sur_P0;

    couverture->profits_and_losses(market_trajectory, p_and_l, pl_sur_P0);
    cout << "Le P&L est : " << p_and_l << endl;
    cout << "Le P&L/P0 est : " << pl_sur_P0 << endl;

//    pnl_vect_free(&initialSpots);
//    pnl_vect_free(&volatilities);
//    pnl_vect_free(&weights);
    pnl_mat_free(&market_trajectory);
    delete couverture;


    exit(0);
}