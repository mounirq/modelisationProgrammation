#include "FakeRandom.hpp"

FakeRandom::FakeRandom():RandomGenerator(){
}


FakeRandom::FakeRandom(const FakeRandom & pnlRandom):RandomGenerator(){
}

FakeRandom::~FakeRandom(){}


/* fill matrix with random values*/
void FakeRandom::fillMatrix(PnlMat *matrix, int nbOfRows, int nbOfCol){
    pnl_mat_set_all(matrix, 1);
}
