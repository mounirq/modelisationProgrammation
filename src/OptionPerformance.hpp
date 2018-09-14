#ifndef OPTION_PERFORMANCE_H
#define OPTION_PERFORMANCE_H
#pragma once

#include "Option.hpp"

/// OptionPerformance implement Option

class OptionPerformance : public Option
{
public:
    PnlVect *weights;
    double strike;

};
#endif
