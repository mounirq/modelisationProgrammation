#include "MonteCarlo.hpp"
#include <math.h>

MonteCarlo::price(double &prix, double &ic)
{
    PnlMat *path;
    double sum = 0;
    double icSum = 0;

    for (i=0 ; i<nbSamples_ ; i++)
    {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        sum += opt_->payoff(path);
        icSum = pow(opt_->payoff(path), 2);
    }
    prix = exp(-mod_->r_ * opt_->T_) * (1/nbSamples_) * sum;

    double varApprochee = exp(- 2 * mod_->r_ * opt_->T_) * ( ((1/nbSamples_) * icSum) - pow((1/nbSamples_) * sum, 2) );
    ic = 2 * 1.96 * sqrt(varApprochee) / sqrt(nbSamples_);

}
