#ifndef OPTION_ASIAN_H
#define OPTION_ASIAN_H
#pragma once

#include "Option.hpp"

/// OptionAsian implement Option

class OptionAsian : public Option
{
public:
    PnlVect *weights;
    double strike;

};
#endif
