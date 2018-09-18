#ifndef BASKET_OPTION_H
#define BASKET_OPTION_H
#pragma once

#include "Option.hpp"

/// BasketOption implement Option

class BasketOption : public Option
{
public:
    PnlVect *weights_;
    double strike_;

    ~BasketOption();

    BasketOption();
    BasketOption(double T_, int nbTimeSteps_, int size_, double strike, PnlVect *weights);
    BasketOption(const BasketOption &basketOption);
    double payoff(const PnlMat *path);
};
#endif