#include <iostream>
#include <string>
#include <pnl/pnl_vector.h>
#include <pnl/pnl_random.h>
#include "../src/BasketOption.hpp"
#include "../src/BlackScholesModel.hpp"
#include "../src/MonteCarlo.hpp"
#include <ctime>


using namespace std;

int main(int argc, char **argv)
{

    int size = 2;
    double strike = 100.0;
    PnlVect* initialSpots = pnl_vect_create_from_scalar(size,100);
    double maturity = 3;
    PnlVect* volatilities = pnl_vect_create_from_scalar(size,0.2);
    double intrestRate = 0.04879;
    double rho = 0.0;
    PnlVect* weights = pnl_vect_create_from_scalar(size,0.025);
    int nbTimeSteps = 1;
    double prix1;
    double ic1;


    int nbSamples = 50000;
    PnlRandom * pnlRandom = new PnlRandom();
    //MonteCarlo *mc = new MonteCarlo(bs, optionBasket, pnlRandom, 0.01, nbSamples);

    //construire MonteCarlo a partir du nom de fichier : cet appel compile mais ne marche pas
    MonteCarlo *mc = new MonteCarlo((char *)"../../data/asian.dat");


    PnlVect *delta = pnl_vect_create_from_scalar(size, 1);
    PnlMat *past = pnl_mat_create_from_scalar(size, nbTimeSteps+1, 100);

    PnlVect * icDelta = pnl_vect_create_from_scalar(size, 0);
    mc->delta(past, 0, delta, icDelta);
    pnl_vect_print(icDelta);
//    pnl_vect_print(delta);

    clock_t  start;
    start = clock();
    cout<< "Time is : " << (clock() - start)/ (double)(CLOCKS_PER_SEC / 1000) << " ms";
    //pnl_vect_print(delta);


    pnl_vect_free(&initialSpots);
    pnl_vect_free(&volatilities);
    pnl_vect_free(&weights);
    pnl_vect_free(&delta);
    pnl_mat_free(&past);
//    delete optionBasket;
//    delete bs;
//    delete mc;

    exit(0);
}
