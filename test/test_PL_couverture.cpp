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
    PnlMat *market_trajectory = pnl_mat_create_from_file((char *)"../../data/market-soutenance/market_call.dat");

    PricerMC *pricerMC = new PricerMC((char *)"../../data/data-soutenance/call.dat");
    PricerBS *pricerBS = new PricerBS((char *)"../../data/data-soutenance/call.dat");


    Couverture *couvertureMC = new Couverture(pricerMC);

    double PL_MC = 0, pl_sur_P0_MC=0;


    couvertureMC->profits_and_losses(market_trajectory, PL_MC, pl_sur_P0_MC);


    cout << "Le P&L par MC est : " << PL_MC << endl;
    cout << "Le P&L/P0 par MC est : " << pl_sur_P0_MC << endl;

    Couverture *couvertureBS = new Couverture(pricerBS);

    cout << "############" << endl;

    double PL_BS = 0, pl_sur_P0_BS=0;

    couvertureBS->profits_and_losses(market_trajectory, PL_BS, pl_sur_P0_BS);

    cout << "Le P&L par BS est : " << PL_BS << endl;
    cout << "Le P&L/P0 par BS est : " << pl_sur_P0_BS << endl;

    pnl_mat_free(&market_trajectory);
    delete couvertureBS;
    delete couvertureMC;

    exit(0);
}