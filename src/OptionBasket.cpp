#include "OptionBasket.hpp"

OptionBasket::~OptionBasket(){
    pnl_vect_free(&weights_);
}

OptionBasket::OptionBasket():Option(){
    weights_ = pnl_vect_new();
    strike_ = 0;
}

OptionBasket::OptionBasket(double T, int nbTimeSteps, int size, double strike, PnlVect *weights):Option(T,nbTimeSteps,size){
    strike_ = strike;
    weights = pnl_vect_copy(const & weights);
}

OptionBasket::OptionBasket(const OptionBasket &optionBasket):Option(T,nbTimeSteps,size){
    strike_ = optionBasket.strike_;
    weights_ = pnl_vect_copy(const & optionBasket.weights)
}

double OptionBasket::payoff(const PnlMat *path) {
    PnlVect * pnlVect = pnl_vect_create(size_);
    int lastRow = nbTimeSteps_ + 1 - 1;
    pnl_mat_get_row(pnlVect, path, lastRow);
    double productScalar = pnl_vect_scalar_prod(const pnlVect, const weights_);
    double payOffReturned = (productScalar - strike_ > 0) ? (productScalar - strike_) : 0;
    return payOffReturned;
}
