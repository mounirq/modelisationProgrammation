#include <iostream>
#include <string>
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

using namespace std;

int main(int argc, char **argv)
{
	cout<<"Début\n";
	int nbTimeSteps = 10;
	int nbUnderlyings = 2;
	double maturity = 1;

	PnlVect * sigma = pnl_vect_create(nbUnderlyings);
	PnlVect * spot = pnl_vect_create(nbUnderlyings);
	pnl_vect_set(sigma, 0, 0.2);
	pnl_vect_set(sigma, 1, 0.1);
	pnl_vect_set(spot, 0, 10);
	pnl_vect_set(spot, 1, 15);
	BlackScholesModel model = BlackScholesModel(nbUnderlyings, 0.1, 0.5, sigma, spot);
	PnlMat * path = pnl_mat_create(nbTimeSteps + 1, nbUnderlyings);
	PnlRng * rng = pnl_rng_create(1);
	pnl_rng_sseed(rng, 0);
	model.asset(path, maturity, nbTimeSteps, rng);
	//pnl_mat_print(path);
	//pnl_vect_print(sigma);
	//pnl_vect_print_asrow(sigma);
	cout<<"Fin\n";
    exit(0);
}
