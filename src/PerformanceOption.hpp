#ifndef PERFORMANCE_OPTION_H
#define PERFORMANCE_OPTION_H
#pragma once

#include "Option.hpp"

/// PerformanceOption implement Option

class PerformanceOption : public Option
{
public:
    PnlVect *weights_;

    ~PerformanceOption();
    PerformanceOption();
    PerformanceOption(double T_, int nbTimeSteps_, int size_, PnlVect *weights);
    PerformanceOption(const PerformanceOption & optionPerformance);
    double payoff(const PnlMat *path);
};
#endif
