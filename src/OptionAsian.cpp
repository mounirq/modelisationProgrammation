#include "OptionAsian.hpp"

OptionAsian::~OptionAsian(){
    pnl_vect_free(&weights_);
}


OptionAsian::OptionAsian():Option(){
    weights_ = pnl_vect_new();
    strike_ = 0;
}

OptionAsian::OptionAsian(double T, int nbTimeSteps, int size, double strike, PnlVect *weights):Option(T,nbTimeSteps,size){
    strike_ = strike;
    weights = pnl_vect_copy(const & weights);
}

OptionAsian::OptionAsian(const OptionAsian &OptionAsian):Option(T,nbTimeSteps,size){
    strike_ = OptionAsian.strike_;
    weights_ = pnl_vect_copy(const & OptionAsian.weights)
}

double OptionAsian::payoff(const PnlMat *path) {
    double sum = 0;
    double lambda = 0;
    PnlVect * pnlVect = pnl_vect_create(nbTimeSteps_ + 1);
    for(int i = 0 ; i < size_, i++){
        lambda = GET(weights_,i);
        pnl_mat_get_col(pnlVect, path,i);
        pnl_vect_mult_scalar(pnlVect, lambda);
        sum += pnl_vect_sum(pnlVect);
    }
    PnlVect * pnlVect = pnl_vect_create(size_);
    int lastRow = nbTimeSteps_ + 1 - 1;
    pnl_mat_get_row(pnlVect, path, lastRow);
    double productScalar = pnl_vect_scalar_prod(const pnlVect, const weights_);
    double payOffReturned = (productScalar - strike_ > 0) ? (productScalar - strike_) : 0;
    return payOffReturned;
}
