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
#include <ctime>

using namespace std;

int main(int argc, char **argv) {

    PricerMC *pricerMC = new PricerMC(argv[1]);
    PnlMat *market_trajectory;
    if(argc == 3){
        market_trajectory = pnl_mat_create_from_file(argv[2]);
    }
    else if (argc == 2){
        market_trajectory = pnl_mat_create(pricerMC->H_ + 1, pricerMC->mod_->size_);
        pricerMC->mod_->simul_market(market_trajectory, pricerMC->opt_->T_, pricerMC->H_,pricerMC->rng_);
    }
    else{
        cerr<<"\nERROR: Number of argument incorrect ";
        throw;
    }

    Couverture *couvertureMC = new Couverture(pricerMC);

    double PL_MC = 0, pl_sur_P0_MC=0;



    clock_t  start;
    start = clock();
    couvertureMC->profits_and_losses(market_trajectory, PL_MC, pl_sur_P0_MC);
    clock_t end = clock();

    cout << "Le P&L par MC est : " << PL_MC << endl;
    cout << "Le P&L/P0 par MC est : " << pl_sur_P0_MC << endl;

    cout<< "The profits and losses took : " << (end - start)/ (double)(CLOCKS_PER_SEC / 1000) << " ms \n";

    cout << "############" << endl;


    pnl_mat_free(&market_trajectory);
    delete couvertureMC;

    exit(0);
}