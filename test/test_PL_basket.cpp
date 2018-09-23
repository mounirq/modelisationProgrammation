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
    int size = 5;
    double strike = 100.0;
    PnlVect *initialSpots = pnl_vect_create_from_scalar(5, 100);
    double maturity = 1;
    PnlVect *volatilities = pnl_vect_create_from_scalar(5, 0.2);
    double intrestRate = 0.04879;
    double rho = 0.0;
    PnlVect *weights = pnl_vect_create_from_scalar(5, 0.2);
    int nbTimeSteps = 1;

    BasketOption *optionBasket = new BasketOption(maturity, nbTimeSteps, size, strike, weights);

    BlackScholesModel *bs = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);

    double prix1;
    double ic1;

    int nbSamples = 50000;

    PnlMat *market_trajectory = pnl_mat_create_from_file((char *)"../../data/market-data/simul_basket.dat");

    //cout << "hell : " << market_trajectory->m << endl;
    PnlRandom * rng = new PnlRandom();

    PricerMC *pricer = new PricerMC(bs, optionBasket, rng, 0.01, nbSamples);

    Couverture *couverture = new Couverture(pricer);


    double p_and_l;
//    mc->profits_and_losses(market_trajectory, p_and_l);
//    cout << "Le P&L est : " << p_and_l << endl;

    couverture->profits_and_losses(market_trajectory, p_and_l);
    cout << "Le P&L est : " << p_and_l << endl;

    pnl_vect_free(&initialSpots);
    pnl_vect_free(&volatilities);
    pnl_vect_free(&weights);
    delete rng;
    delete pricer;
    delete couverture;
    pnl_mat_free(&market_trajectory);
//    delete optionBasket;
//    delete bs;
//    delete mc;

    exit(0);
}