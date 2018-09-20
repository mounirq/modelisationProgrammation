#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "../src/BasketOption.hpp"
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
    BasketOption *optionBasket = new BasketOption(maturity, nbTimeSteps, size, strike, weights);

    BlackScholesModel *bs = new BlackScholesModel(size, intrestRate, rho, volatilities, initialSpots);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);

    pnl_rng_sseed(rng, time(NULL));

    double prix1;
    double ic1;

    int nbSamples = 50000;

    MonteCarlo *mc = new MonteCarlo(bs, optionBasket, rng, 0.01, nbSamples);

    //construire MonteCarlo a partir du nom de fichier : cet appel compile mais ne marche pas
    MonteCarlo *mc1 = new MonteCarlo((char *)"../../data/basket_1.dat");

//    mc->price(prix1,ic1);
    mc1->price(prix1,ic1);
    cout << "Le prix à t=0 est : " << prix1 << endl;

    cout << "La largeur de l'intervalle de confiance est : " << ic1 << endl;

    PnlVect * delta = pnl_vect_create(size);
    PnlMat * past = pnl_mat_new();
    mc1->delta(past, 0, delta);
    pnl_vect_print(delta);

    exit(0);
}
