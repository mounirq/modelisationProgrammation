#ifndef OPTION_ASIAN_H
#define OPTION_ASIAN_H
#pragma once

#include "Option.hpp"

/// OptionAsian implement Option

class OptionAsian : public Option
{
public:
    PnlVect *weights_;
    double strike_;

    ~OptionAsian();
    OptionAsian();
    OptionAsian(double T_, int nbTimeSteps_, int size_, double strike, PnlVect *weights);
    OptionAsian(const OptionAsian & optionAsian);
    double payoff(const PnlMat *path);
};
#endif