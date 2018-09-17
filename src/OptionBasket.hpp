#ifndef OPTION_BASKET_H
#define OPTION_BASKET_H
#pragma once

#include "Option.hpp"

/// OptionBasket implement Option

class OptionBasket : public Option
{
public:
    PnlVect *weights_;
    double strike_;

    ~OptionBasket();

    OptionBasket();
    OptionBasket(double T_, int nbTimeSteps_, int size_, double strike, PnlVect *weights);
    OptionBasket(const OptionBasket &optionBasket);
    double payoff(const PnlMat *path);
};
#endif