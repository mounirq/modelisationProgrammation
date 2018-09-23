//#include <iostream>
//#include <string>
//#include <pnl/pnl_vector.h>
//#include <pnl/pnl_random.h>
//#include "../src/BasketOption.hpp"
//#include "../src/BlackScholesModel.hpp"
//#include "../src/MonteCarlo.hpp"
//
//using namespace std;
//
//int main(int argc, char **argv) {
//    int size = 40;
//    int nbTimeSteps = 1;
//
//    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
//
//    pnl_rng_sseed(rng, time(NULL));
//
//    double prix1;
//    double ic1;
//
//    //construire MonteCarlo a partir du nom de fichier : cet appel compile mais ne marche pas
//    MonteCarlo *mc1 = new MonteCarlo((char *) "../../data/basket_1.dat");
//
//    mc1->price(prix1, ic1);
//
//    cout << "Le prix à t=0 est : " << prix1 << endl;
//
//    cout << "La largeur de l'intervalle de confiance est : " << ic1 << endl;
//
//    PnlVect *delta = pnl_vect_create_from_scalar(size, 1);
//    PnlMat *past = pnl_mat_create_from_scalar(size, nbTimeSteps + 1, 100);
//    mc1->delta(past, 0, delta);
//    pnl_vect_print(delta);
//
//    pnl_vect_free(&delta);
//    pnl_mat_free(&past);
//
//    exit(0);
//}