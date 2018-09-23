//
// Created by Yasmine Tidane on 21/09/2018.
//

#ifndef PROJETPRICERGIT_PRICERMC_HPP
#define PROJETPRICERGIT_PRICERMC_HPP


#include "MonteCarlo.hpp"
#include "AbstractPricer.hpp"

class PricerMC : public AbstractPricer
{
public:

    MonteCarlo *mc_;

    ~PricerMC();
    PricerMC(char *fileName);
    PricerMC(BlackScholesModel *model, Option *option, RandomGenerator *rng, double fdStep, size_t nbSamples);

    void price(double &prix, double &ic);

    void price(const PnlMat *past, double t, double &prix, double &ic);

    void delta(const PnlMat *past, double t, PnlVect *delta);

};


#endif //PROJETPRICERGIT_PRICERMC_HPP
