#include "PerformanceOption.hpp"
#include <iostream>
using namespace std;

PerformanceOption::~PerformanceOption(){
    pnl_vect_free(&weights_);
}


PerformanceOption::PerformanceOption():Option(){
    weights_ = pnl_vect_new();
}

PerformanceOption::PerformanceOption(double T, int nbTimeSteps, int size, PnlVect *weights):Option(T,nbTimeSteps,size){
    weights_ = pnl_vect_copy(weights);
}

PerformanceOption::PerformanceOption(const PerformanceOption &performanceOption):Option(performanceOption.T_,performanceOption.nbTimeSteps_,performanceOption.size_){
    weights_ = pnl_vect_copy(performanceOption.weights_);
}

double PerformanceOption::payoff(const PnlMat *path) {
    PnlVect * vect = pnl_mat_mult_vect(path, weights_);
    PnlVect *vectTmp1 = pnl_vect_create_subvect(vect, 1, nbTimeSteps_);
    PnlVect *vectTmp2 = pnl_vect_create_from_scalar(nbTimeSteps_, 0);
    pnl_vect_extract_subvect(vectTmp2, vect, 0,nbTimeSteps_);
    pnl_vect_div_vect_term(vectTmp1, vectTmp2);
    pnl_vect_minus_scalar(vectTmp1, 1);
    double maximumValue = 0;
    double tmpValue = 0;
    for(int i = 0; i < nbTimeSteps_; i++ ){
        tmpValue = GET(vectTmp1, i);
        maximumValue = (tmpValue > 0) ? tmpValue : 0;
        LET(vectTmp1, i) = maximumValue;
    }
    double payoffReturned = pnl_vect_sum(vectTmp1);

    pnl_vect_free(&vect);
    pnl_vect_free(&vectTmp1);
    pnl_vect_free(&vectTmp2);

    return payoffReturned+1;
}