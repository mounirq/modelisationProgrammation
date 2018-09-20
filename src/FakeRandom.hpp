#ifndef FAKERANDOM_HPP
#define FAKERANDOM_HPP
#pragma once

#include "RandomGenerator.hpp"

class FakeRandom : public RandomGenerator {
public:

    FakeRandom();

    FakeRandom(const FakeRandom & fakeRandom);

    ~FakeRandom();

    /* fill all the matrix with "1"*/
    void fillMatrix(PnlMat * matrix, int nbOfRows, int nbOfCol);

};


#endif
