#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "../src/BasketOption.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/MonteCarlo.hpp"
#include "../src/PricerMC.hpp"
#include "../src/PricerBS.hpp"
#include "../src/Couverture.hpp"

using namespace std;

int main(int argc, char **argv) {
    int size = 1;
    double strike = 100.0;
    PnlVect *initialSpots = pnl_vect_create_from_scalar(1, 100);
    double maturity = 1;
    PnlVect *volatilities = pnl_vect_create_from_scalar(1, 0.2);
    double intrestRate = 0.04879;
    double rho = 0.0;
    PnlVect *weights = pnl_vect_create_from_scalar(1, 1);
    int nbTimeSteps = 1;

    BasketOption *optionCall1 = new BasketOption(maturity, nbTimeSteps, size, strike, weights);
    BasketOption *optionCall2 = new BasketOption(maturity, nbTimeSteps, size, strike, weights);

    BlackScholesModel *bs1 = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);
    BlackScholesModel *bs2 = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);

    int nbSamples = 50000;

    int H = 366;

    PnlMat *market_trajectory = pnl_mat_create_from_file((char *)"../../data/market-data/simul_call.dat");

    PnlRandom * rng = new PnlRandom();

    PricerMC *pricerMC = new PricerMC(bs1, optionCall1, rng, 0.01, nbSamples, H);
    PricerBS *pricerBS = new PricerBS(bs2, optionCall2, rng, 0.01, nbSamples, H);

    double prixMC, icMC;
    double prixBS, icBS;

    pricerMC->price(prixMC, icMC);
    pricerBS->price(prixBS, icBS);

    cout << "Le prix en 0 par MC est : " << prixMC << endl;

    cout << "Le prix en 0 par BS est : " << prixBS << endl;

    cout << "*********************************" << endl;

    PnlMat * past = pnl_mat_create(3, 1);

    pnl_mat_set(past, 0, 0, 100);
    pnl_mat_set(past, 1, 0, 110);
    pnl_mat_set(past, 2, 0, 100);


    PnlVect *deltaMC = pnl_vect_create_from_scalar(1, 0);
    PnlVect *deltaBS = pnl_vect_create_from_scalar(1, 0);
    PnlVect *icDelta = pnl_vect_create_from_scalar(1, 0);
    pricerBS->delta(past, 0, deltaBS, icDelta);
    pricerMC->delta(past, 0, deltaMC, icDelta);

    cout << "Le delta en t=0 par MC est : " << endl;

    pnl_vect_print(deltaMC);

    cout << "Le delta en t=0 par BS est : " << endl;

    pnl_vect_print(deltaBS);

    cout << "*********************************" << endl;

    Couverture *couvertureMC = new Couverture(pricerMC);

    double PL_MC = 0, pl_sur_P0_MC=0;


    couvertureMC->profits_and_losses(market_trajectory, PL_MC, pl_sur_P0_MC);


    cout << "Le P&L par MC est : " << PL_MC << endl;
    cout << "Le P&L/P0 par MC est : " << pl_sur_P0_MC << endl;

    Couverture *couvertureBS = new Couverture(pricerBS);

    cout << "############" << endl;

    double PL_BS = 0, pl_sur_P0_BS=0;

    couvertureBS->profits_and_losses(market_trajectory, PL_BS, pl_sur_P0_BS);

    cout << "Le P&L par BS est : " << PL_BS << endl;
    cout << "Le P&L/P0 par BS est : " << pl_sur_P0_BS << endl;


    pnl_vect_free(&initialSpots);
    pnl_vect_free(&volatilities);
    pnl_vect_free(&weights);
    pnl_mat_free(&market_trajectory);
    pnl_vect_free(&deltaMC);
    pnl_vect_free(&deltaBS);
    pnl_mat_free(&past);

    delete couvertureBS;
    delete couvertureMC;

    exit(0);
}