#include <iostream>
#include <string>
#include "../src/BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "../src/FakeRandom.hpp"

using namespace std;

int main(int argc, char **argv)
{
	cout<<"Début\n";
	int nbTimeSteps = 5;
	int nbUnderlyings = 2;
	double maturity = 1;

	PnlVect * sigma = pnl_vect_create(nbUnderlyings);
	PnlVect * spot = pnl_vect_create(nbUnderlyings);
	pnl_vect_set(sigma, 0, 0.25);
	pnl_vect_set(sigma, 1, 0.20);
	pnl_vect_set(spot, 0, 10);
	pnl_vect_set(spot, 1, 15);
    // Fill path in t = 0
	BlackScholesModel model = BlackScholesModel(nbUnderlyings, 0.03, 0.15, sigma, spot);
	PnlMat * path = pnl_mat_create(nbTimeSteps + 1, nbUnderlyings);
	FakeRandom * fakeRandom = new FakeRandom();
	model.asset(path, maturity, nbTimeSteps, fakeRandom);
    cout << "The spot matrix calculated by asset is : \n";
	pnl_mat_print(path);
    PnlMat * corrMat = pnl_mat_create_from_scalar(nbUnderlyings, nbUnderlyings, 0.15);
    for(int i=0; i<nbUnderlyings; i++)
    {
        pnl_mat_set(corrMat, i, i, 1);
    }
    int result = pnl_mat_chol(corrMat);
    double L_1G = pnl_mat_get(corrMat,0,0) +pnl_mat_get(corrMat,0,1) ;
    double L_2G = pnl_mat_get(corrMat,1,0) + pnl_mat_get(corrMat,1,1) ;
    PnlVect *lVector = pnl_vect_create_from_list(2, L_1G, L_2G);
    /* t = T/5 = 1/5 */
    PnlMat *matrixSpot = pnl_mat_create(6,2);
    pnl_mat_set(matrixSpot, 0, 0, 10);
    pnl_mat_set(matrixSpot, 0, 1, 15);
    double tmp;
    for(int i = 1; i < 6; i++){
        for(int j = 0; j < 2; j++){
            tmp = pnl_mat_get(matrixSpot,i-1,j)*exp((0.03 - (pnl_vect_get(sigma,j)*pnl_vect_get(sigma,j))/2)*0.2 + pnl_vect_get(sigma,j)* sqrt(0.2) * pnl_vect_get(lVector,j));
            pnl_mat_set(matrixSpot, i, j,tmp);
        }
    }
    cout << "The spot matrix must be equal to : \n";
    pnl_mat_print(matrixSpot);

/*	PnlMat * shift_path = pnl_mat_create(nbTimeSteps+1, nbUnderlyings);
	cout<<"--------------\n";
	model.shiftAsset(shift_path, path, 0, 1, 20, maturity/nbTimeSteps);
	pnl_mat_print(shift_path);
	cout<<"Fin\n";*/
    exit(0);
}
