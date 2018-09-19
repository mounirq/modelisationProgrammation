#ifndef PNLRANDOM_H
#define PNLRANDOM_H
#pragma once

#include "RandomGenerator.hpp"


class PnlRandom : public RandomGenerator{
public:
    PnlRng * generator_;

    PnlRandom();

    PnlRandom(const PnlRandom & pnlRandom);

    ~PnlRandom();

    /* fill matrix with random values*/
    void fillMatrix(PnlMat * matrix, int nbOfRows, int nbOfCol);
};


#endif