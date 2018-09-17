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
    weights = pnl_vect_copy(weights);
}

OptionAsian::OptionAsian(const OptionAsian &optionAsian):Option(optionAsian.T_,optionAsian.nbTimeSteps_,optionAsian.size_){
    strike_ = optionAsian.strike_;
    weights_ = pnl_vect_copy(optionAsian.weights_);
}

double OptionAsian::payoff(const PnlMat *path) {
    double sum = 0;
    double lambda = 0;
    PnlVect * pnlVect = pnl_vect_create(nbTimeSteps_ + 1);
    for(int i = 0 ; i < size_; i++){
        lambda = GET(weights_,i);
        pnl_mat_get_col(pnlVect, path,i);
        pnl_vect_mult_scalar(pnlVect, lambda);
        sum += pnl_vect_sum(pnlVect);
    }
    sum /= (nbTimeSteps_ + 1);
    double payOffReturned = (sum - strike_ > 0) ? (sum - strike_) : 0;
    return payOffReturned;
}
