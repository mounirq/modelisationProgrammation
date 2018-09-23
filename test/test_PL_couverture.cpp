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

    BasketOption *optionCall = new BasketOption(maturity, nbTimeSteps, size, strike, weights);

    BlackScholesModel *bs = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);

    int nbSamples = 50000;

    PnlMat *market_trajectory = pnl_mat_create_from_file((char *)"../../data/market-data/simul_call.dat");

    //cout << "hell : " << market_trajectory->m << endl;
    PnlRandom * rng = new PnlRandom();

    PricerMC *pricerMC = new PricerMC(bs, optionCall, rng, 0.01, nbSamples);
    PricerBS *pricerBS = new PricerBS(bs, optionCall, rng, 0.01, nbSamples);

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

    PnlMat *sub_past = pnl_mat_new();
    pnl_mat_extract_subblock(sub_past, past, 0, 2, 0, 1);
    pricerMC->price(sub_past, 0.4, prixMC, icMC);
    pricerBS->price(sub_past, 0.4, prixBS, icBS);

//    cout << "Le prix en t=1 par MC est : " << prixMC << endl;
//
//    cout << "Le prix en t=1 par BS est : " << prixBS << endl;
//
//    cout << "*********************************" << endl;

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

    double PL_MC = 0;

    couvertureMC->profits_and_losses(market_trajectory, PL_MC);

    cout << "Le P&L par MC est : " << PL_MC << endl;

    Couverture *couvertureBS = new Couverture(pricerBS);

    cout << "############" << endl;

    double PL_BS = 0;

    couvertureBS->profits_and_losses(market_trajectory, PL_BS);

    cout << "Le P&L par BS est : " << PL_BS << endl;


    pnl_vect_free(&initialSpots);
    pnl_vect_free(&volatilities);
    pnl_vect_free(&weights);
    pnl_mat_free(&market_trajectory);
    pnl_vect_free(&deltaMC);
    pnl_vect_free(&deltaBS);
    pnl_mat_free(&past);
    pnl_mat_free(&sub_past);

    delete rng;
    delete pricerBS;
    delete &pricerMC;
    delete couvertureBS;
    delete &couvertureMC;

    exit(0);
}