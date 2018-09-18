#include "PerformanceOption.hpp"

PerformanceOption::~PerformanceOption(){
    pnl_vect_free(&weights_);
}


PerformanceOption::PerformanceOption():Option(){
    weights_ = pnl_vect_new();
    strike_ = 0;
}

PerformanceOption::PerformanceOption(double T, int nbTimeSteps, int size, double strike, PnlVect *weights):Option(T,nbTimeSteps,size){
    strike_ = strike;
    weights_ = pnl_vect_copy(weights);
}

PerformanceOption::PerformanceOption(const PerformanceOption &performanceOption):Option(performanceOption.T_,performanceOption.nbTimeSteps_,performanceOption.size_){
    strike_ = performanceOption.strike_;
    weights_ = pnl_vect_copy(performanceOption.weights_);
}

double PerformanceOption::payoff(const PnlMat *path) {
    PnlVect *vect ;
    PnlVect *tmp1;
    PnlVect *tmp2;
    vect = pnl_mat_mult_vect(path, weights_);
    tmp1 = pnl_vect_create_subvect(vect, 1, nbTimeSteps_);
    pnl_vect_extract_subvect(tmp2, vect, 0,nbTimeSteps_-1);
    pnl_vect_div_vect_term(tmp1, tmp2);
    pnl_vect_minus_scalar(tmp1, 1);
    double sum = pnl_vect_sum(tmp1);
    double payoffReturned = (sum > 0) ? sum : 0 ;
    return payoffReturned+1;
}