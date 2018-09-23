#include "PricerMC.hpp"

PricerMC::PricerMC(char *fileName) : AbstractPricer(fileName)
{
    mc_ = new MonteCarlo(mod_, opt_, rng_, fdStep_, nbSamples_);
}

PricerMC::PricerMC(BlackScholesModel *model, Option *option, RandomGenerator *rng, double fdStep, size_t nbSamples) : AbstractPricer(model, option, rng, fdStep, nbSamples)
{
    mc_ = new MonteCarlo(mod_, opt_, rng_, fdStep_, nbSamples_);
}

PricerMC::~PricerMC()
{
    delete mod_;
    delete opt_;
    delete rng_;
}

void PricerMC::price(double &prix, double &ic)
{
    mc_->price(prix, ic);
}

void PricerMC::price(const PnlMat *past, double t, double &prix, double &ic)
{
    mc_->price(past, t, prix, ic);
}

void PricerMC::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *icDelta)
{
    mc_->delta(past, t, delta, icDelta);
}