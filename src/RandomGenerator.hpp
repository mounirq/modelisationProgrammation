#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H
#pragma once

#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


class RandomGenerator {
public:

    RandomGenerator();

    RandomGenerator(const RandomGenerator * randomGenerator);

    virtual ~RandomGenerator();

    /* fill matrix with random values*/
    virtual void fillMatrix(PnlMat * matrix, int nbOfRows, int nbOfCol);
};


#endif