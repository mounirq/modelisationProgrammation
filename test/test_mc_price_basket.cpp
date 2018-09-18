#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "../src/OptionBasket.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/MonteCarlo.hpp"

using namespace std;

int main(int argc, char **argv)
{
    int size = 40;
    double strike = 100.0;
    PnlVect* initialSpots = pnl_vect_create_from_scalar(40,100);
    double maturity = 3;
    PnlVect* volatilities = pnl_vect_create_from_scalar(40,0.2);
    double intrestRate = 0.04879;
    double rho = 0.0;
    PnlVect* weights = pnl_vect_create_from_scalar(40,0.025);
    int nbTimeSteps = 1;
    OptionBasket *optionBasket = new OptionBasket(maturity, nbTimeSteps, size, strike, weights);

    BlackScholesModel *bs = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);

    pnl_rng_sseed(rng, time(NULL));

    double prix1;
    double ic1;
//    double prix2;
//    double ic2;

    int nbSamples = 50000;

    //PnlMat* past = pnl_mat_create_from_scalar(1,40,100.0);
    MonteCarlo *mc = new MonteCarlo(bs, optionBasket, rng, 0.01, nbSamples);
//    MonteCarlo *mc2 = new MonteCarlo(bsm,myBasket2,rng,0.1,nbSamples);
//    mc2->price(past,pricingDate,prix2,ic2);
    mc->price(prix1,ic1);
    cout << "Le prix à t=0 est : " << prix1 << endl;
//    cout << prix2 << endl;
    cout << "La largeur de l'intervalle de confiance est : " << ic1 << endl;
//    cout << ic2 << endl;

    exit(0);
}
