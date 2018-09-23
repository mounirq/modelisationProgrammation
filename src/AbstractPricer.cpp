//
// Created by Yasmine Tidane on 21/09/2018.
//

#include <iostream>
#include <jlparser/parser.hpp>
#include "AbstractPricer.hpp"
#include "PnlRandom.hpp"

AbstractPricer::~AbstractPricer() {}

AbstractPricer::AbstractPricer(BlackScholesModel *mod, Option *opt, RandomGenerator *rng, double fdStep, size_t nbSamples) {
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
}

AbstractPricer::AbstractPricer(char *fileName) {
    Param *P = new Parser(fileName);

    size_t size1;
    int size;
    double r;
    double rho;
    PnlVect *sigma, *spot;
    fdStep_ = 0.1;

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
//    rng_ = pnl_rng_create(PNL_RNG_MERSENNE);
//    pnl_rng_sseed(rng_, time(NULL));

    rng_ = new PnlRandom();

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
    pnl_vect_free(&weights);
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    delete P;
}

void AbstractPricer::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *icDelta) {}

void AbstractPricer::price(double &prix, double &ic) {}

void AbstractPricer::price(const PnlMat *past, double t, double &prix, double &ic) {}