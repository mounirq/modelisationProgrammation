#ifndef ASIAN_OPTION_H
#define ASIAN_OPTION_H
#pragma once

#include "Option.hpp"

/// AsianOption implement Option

class AsianOption : public Option
{
public:
    PnlVect *weights_;
    double strike_;

    ~AsianOption();
    AsianOption();
    AsianOption(double T_, int nbTimeSteps_, int size_, double strike, PnlVect *weights);
    AsianOption(const AsianOption & AsianOption);
    double payoff(const PnlMat *path);
};
#endif