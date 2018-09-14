#ifndef OPTION_BASKET_H
#define OPTION_BASKET_H
#pragma once

#include "Option.hpp"

/// OptionBasket implement Option

class OptionBasket : public Option
{
public:
    PnlVect *weights;
    double strike;

    ~OptionBasket();

    OptionBasket();
    Optionbasket(double T_, int nbTimeSteps_, int size_, double strike, PnlVect *weights);

};
#endif
