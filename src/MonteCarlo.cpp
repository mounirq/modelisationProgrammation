#include "MonteCarlo.hpp"
#include <stdio.h>
#include <string>

/** Constructeur par défaut */
//MonteCarlo::MonteCarlo() {
//    mod_ = new BlackScholesModel();
//    opt_ = new Option();
//    rng_ = pnl_rng_new();
//    fdStep_ = 0;
//    nbSamples_ = 0;
//}


MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples)
{
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
}

/** Constructeur par recopie */
MonteCarlo::MonteCarlo(const MonteCarlo &MC)
{
    mod_ = new BlackScholesModel(*(MC.mod_));

    //opt_ = new Option(*(MC.opt_));
};


MonteCarlo::~MonteCarlo() {
    free(mod_);
    free(opt_);
    free(rng_);
}

void MonteCarlo::price(double &prix, double &ic)
{
    PnlMat *path;
    path= pnl_mat_create(opt_->nbTimeSteps_+1,mod_->size_);

    double sum = 0;
    double icSum = 0;
    double payoffTmp = 0;

    for (int i=0 ; i<nbSamples_ ; i++)
    {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);

        payoffTmp = opt_->payoff(path);
        sum += payoffTmp;
        icSum = payoffTmp * payoffTmp;
    }
    prix = (exp(-mod_->r_ * opt_->T_)/nbSamples_) * sum;

    double varApprochee = exp(- 2 * mod_->r_ * opt_->T_) * ( ((1.0/nbSamples_) * icSum) - ((1.0/nbSamples_) * sum) * ((1.0/nbSamples_) * sum) );
    ic = 2 * 1.96 * sqrt(varApprochee) / sqrt(nbSamples_);

    pnl_mat_free(&path);

}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic)
{
    int nbActifs = mod_->size_;
    PnlMat *path;
    path= pnl_mat_create(opt_->nbTimeSteps_+1,nbActifs);

    double sum = 0;
    double icSum = 0;
    double payoffTmp = 0;

    for (int i=0 ; i<nbSamples_ ; i++)
    {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        payoffTmp = opt_->payoff(path);
        sum += payoffTmp;
        icSum = payoffTmp * payoffTmp;
    }
    prix = (exp(-mod_->r_ * opt_->T_)/nbSamples_) * sum;

    double varApprochee = exp(- 2 * mod_->r_ * opt_->T_) * ( ((1.0/nbSamples_) * icSum) - ((1.0/nbSamples_) * sum) * ((1.0/nbSamples_) * sum) );
    ic = 2 * 1.96 * sqrt(varApprochee) / sqrt(nbSamples_);

    pnl_mat_free(&path);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta) {
    int nbActifs = mod_->size_;
    PnlMat *shift_plus_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
    PnlMat *shift_moins_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
    PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
    PnlVect *vect_sum = pnl_vect_create(nbActifs);

    double timestep = opt_->T_ / opt_->nbTimeSteps_;

    for (int j = 0; j < nbSamples_; j++) {
        //calcul de la bonne (t=0 ou t>0) trajectoire du sous-jacent
        if (t == 0) {
            mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        } else {
            mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        }

        for (int d = 0; d < nbActifs; d++) {

            mod_->shiftAsset(shift_plus_path, path, d, fdStep_, t, timestep);

            mod_->shiftAsset(shift_moins_path, path, d, -fdStep_, t, timestep);

//            vect_sum[d] += opt_->payoff(shift_plus_path) - opt_->payoff(shift_moins_path);
            LET(vect_sum, d) = GET(vect_sum, d) + opt_->payoff(shift_plus_path) - opt_->payoff(shift_moins_path);
        }
    }

    for (int d = 0; d < nbActifs; d++) {
        if (t == 0) {
            LET(delta, d) = GET(vect_sum, d) * exp(-mod_->r_ * (opt_->T_ - t)) /
                            (2.0 * nbSamples_ * MGET(path, 0, d) * fdStep_);
        } else {
            LET(delta, d) = GET(vect_sum, d) * exp(-this->mod_->r_ * (this->opt_->T_ - t)) /
                            (2.0 * nbSamples_ * MGET(past, past->m - 1, d) * fdStep_);
        }
    }
    pnl_mat_free(&shift_plus_path);
    pnl_mat_free(&shift_moins_path);
    pnl_mat_free(&path);
    pnl_vect_free(& vect_sum);
}