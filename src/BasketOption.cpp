#include "BasketOption.hpp"

BasketOption::~BasketOption(){
    pnl_vect_free(&weights_);
}

BasketOption::BasketOption():Option(){
    weights_ = pnl_vect_new();
    strike_ = 0;
}

BasketOption::BasketOption(double T, int nbTimeSteps, int size, double strike, PnlVect *weights):Option(T,nbTimeSteps,size){
    strike_ = strike;
    weights_ = pnl_vect_copy(weights);
}

BasketOption::BasketOption(const BasketOption &basketOption):Option(basketOption.T_,basketOption.nbTimeSteps_,basketOption.size_){
    strike_ = basketOption.strike_;
    weights_ = pnl_vect_copy(basketOption.weights_);
}

double BasketOption::payoff(const PnlMat *path) {
    PnlVect * pnlVect = pnl_vect_create(size_);
    int lastRow = nbTimeSteps_ + 1 - 1;
    pnl_mat_get_row(pnlVect, path, lastRow);
    double productScalar = pnl_vect_scalar_prod(pnlVect, weights_);
    double payOffReturned = (productScalar - strike_ > 0) ? (productScalar - strike_) : 0;
    return payOffReturned;
}
