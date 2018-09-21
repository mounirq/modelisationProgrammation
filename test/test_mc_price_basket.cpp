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
    int size = 40;

    PnlVect* initialSpots = pnl_vect_create_from_scalar(40,100);
    PnlVect* volatilities = pnl_vect_create_from_scalar(40,0.2);

    PnlVect* weights = pnl_vect_create_from_scalar(40,0.025);
    int nbTimeSteps = 1;
    double prix1;
    double ic1;

    MonteCarlo *mc1 = new MonteCarlo((char *) "../../data/basket_1.dat");

    mc1->price(prix1,ic1);
    cout << "Le prix à t=0 est : " << prix1 << endl;

    cout << "La largeur de l'intervalle de confiance est : " << ic1 << endl;

    PnlVect *delta = pnl_vect_create_from_scalar(size, 1);
    PnlMat *past = pnl_mat_create_from_scalar(size, nbTimeSteps+1, 100);
    clock_t  start;
    start = clock();
    mc1->delta(past, 0, delta);
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