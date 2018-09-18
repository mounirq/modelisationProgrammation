#include "AsianOption.hpp"

AsianOption::~AsianOption(){
    pnl_vect_free(&weights_);
}


AsianOption::AsianOption():Option(){
    weights_ = pnl_vect_new();
    strike_ = 0;
}

AsianOption::AsianOption(double T, int nbTimeSteps, int size, double strike, PnlVect *weights):Option(T,nbTimeSteps,size){
    strike_ = strike;
    weights_ = pnl_vect_copy(weights);
}

AsianOption::AsianOption(const AsianOption &asianOption):Option(asianOption.T_,asianOption.nbTimeSteps_,asianOption.size_){
    strike_ = asianOption.strike_;
    weights_ = pnl_vect_copy(asianOption.weights_);
}

double AsianOption::payoff(const PnlMat *path) {
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
