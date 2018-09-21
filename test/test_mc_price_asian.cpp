#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "../src/AsianOption.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/MonteCarlo.hpp"

using namespace std;

int main(int argc, char **argv)
{
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

//    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
//
//    pnl_rng_sseed(rng, time(NULL));

    double prix1;
    double ic1;

    int nbSamples = 50000;

    PnlRandom *rng = new PnlRandom();

    MonteCarlo *mc = new MonteCarlo(bs, asianOption, rng, 0.01, nbSamples);

    mc->price(prix1,ic1);
    cout << "Le prix à t=0 est : " << prix1 << endl;

    cout << "La largeur de l'intervalle de confiance est : " << ic1 << endl;


    exit(0);
}
