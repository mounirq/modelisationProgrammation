#include <iostream>
#include <string>
#include "../src/BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

using namespace std;

int main(int argc, char **argv)
{
	cout<<"Début\n";
	int nbTimeSteps = 5;
	int nbUnderlyings = 2;
	double maturity = 2;

	PnlVect * sigma = pnl_vect_create(nbUnderlyings);
	PnlVect * spot = pnl_vect_create(nbUnderlyings);
	pnl_vect_set(sigma, 0, 0.25);
	pnl_vect_set(sigma, 1, 0.20);
	pnl_vect_set(spot, 0, 10);
	pnl_vect_set(spot, 1, 15);
	BlackScholesModel model = BlackScholesModel(nbUnderlyings, 0.03, 0.15, sigma, spot);
	PnlMat * path = pnl_mat_create(nbTimeSteps + 1, nbUnderlyings);
	PnlRng * rng = pnl_rng_create(1);
	pnl_rng_sseed(rng, time(NULL));
	model.asset(path, maturity, nbTimeSteps, rng);
	//pnl_mat_print(path);
	//pnl_vect_print(sigma);
	//pnl_vect_print_asrow(sigma);

	PnlMat * past = pnl_mat_create(3, nbUnderlyings);
	pnl_mat_set(past, 0, 0, 10);
	pnl_mat_set(past, 0, 1, 15);
	pnl_mat_set(past, 1, 0, 10.292682);
	pnl_mat_set(past, 1, 1, 15.169634);
	pnl_mat_set(past, 2, 0, 10.263704);
	pnl_mat_set(past, 2, 1, 15.039295);
	//pnl_mat_print(past);
	//model.asset(path, 1, maturity, nbTimeSteps, rng, past);
	pnl_mat_print(path);

	cout<<"Fin\n";
    exit(0);
}
