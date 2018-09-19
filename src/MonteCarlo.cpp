#include <iostream>
#include <string>
#include "MonteCarlo.hpp"
#include "jlparser/parser.hpp"
#include "BasketOption.hpp"
#include "AsianOption.hpp"
#include "PerformanceOption.hpp"

MonteCarlo::MonteCarlo(char *fileName)
{
    Param *P = new Parser(fileName);

    size_t size1;
    int size;
    double r;
    double rho;
    PnlVect *sigma, *spot;

    P->extract("sample number", this->nbSamples_);
    P->extract("option size", size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("volatility", sigma, size);
    P->extract("spot", spot, size);
    this->mod_ = new BlackScholesModel(size, r, rho, sigma, spot);

    double T;
    int nbTimeSteps;
    PnlVect *weights;
    std::string optionType;
    P->extract("option type", optionType);

    P->extract("maturity", T);
    P->extract("timestep number", nbTimeSteps);
    P->extract("payoff coefficients", weights, size);
    rng_ = pnl_rng_create(PNL_RNG_MERSENNE);

    pnl_rng_sseed(rng_, time(NULL));

    //decommenter les lignes commentees si l'option performance marche

    if (optionType.compare("performance") == 0 )
    {
        this->opt_ = new PerformanceOption(T, nbTimeSteps, size, weights);
    }
    else {
        double strike;
        P->extract("strike", strike);
        if (optionType.compare("basket") == 0 )
        {
            this->opt_ = new BasketOption(T, nbTimeSteps, size, strike, weights);
        }
        else if (optionType.compare("asian") == 0 )
        {
            this->opt_ = new AsianOption(T, nbTimeSteps, size, strike, weights);
        }
        else {
            std::cerr << "Unkonwn option type" << std::endl;
        }
    }
}

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, size_t nbSamples)
{
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
}

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
        icSum += payoffTmp * payoffTmp;
    }
    prix = (exp(-mod_->r_ * opt_->T_)/nbSamples_) * sum;

    double varApprochee = exp(- 2 * mod_->r_ * opt_->T_) * ( ((1.0/nbSamples_) * icSum) - (((1.0/nbSamples_) * sum) * ((1.0/nbSamples_) * sum)) );


    ic = 2 * 1.96 * sqrt(varApprochee/nbSamples_);

    std::cout << "standard deviation Approchee : " << sqrt(varApprochee/nbSamples_) << std::endl;

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
    ic = 2 * 1.96 * sqrt(varApprochee/nbSamples_);

    pnl_mat_free(&path);
}

//void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta) {
//    int nbActifs = mod_->size_;
//    PnlMat *shift_plus_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
//    PnlMat *shift_moins_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
//    PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
//    PnlVect *vect_sum = pnl_vect_create(nbActifs);
//
//    double timeStep = opt_->T_ / opt_->nbTimeSteps_;
//
//    for (int j = 0; j < nbSamples_; j++) {
//        if (t == 0) {
//            mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
//        } else {
//            mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
//        }
//
//        for (int d = 0; d < nbActifs; d++) {
//
//            mod_->shiftAsset(shift_plus_path, path, d, fdStep_, t, timeStep);
//
//            mod_->shiftAsset(shift_moins_path, path, d, -fdStep_, t, timeStep);
//
//            pnl_vect_set(vect_sum, d, pnl_vect_get(vect_sum, d) + opt_->payoff(shift_plus_path) - opt_->payoff(shift_moins_path));
//        }
//    }
//
//    for (int d = 0; d < nbActifs; d++) {
//        if (t == 0) {
//            pnl_vect_set(delta, d, pnl_vect_get(vect_sum, d) * exp(-mod_->r_ * (opt_->T_ - t))/(2.0 * nbSamples_ * pnl_mat_get(path, 0, d)* fdStep_));
//        } else {
//            pnl_vect_set(delta, d, pnl_vect_get(vect_sum, d) * exp(-mod_->r_ * (opt_->T_ - t))/(2.0 * nbSamples_ * pnl_mat_get(path, past->m - 1, d) * fdStep_));
//        }
//    }
//    pnl_mat_free(&shift_plus_path);
//    pnl_mat_free(&shift_moins_path);
//    pnl_mat_free(&path);
//    pnl_vect_free(& vect_sum);
//}
