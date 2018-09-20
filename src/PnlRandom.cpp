#include "PnlRandom.hpp"

PnlRandom::PnlRandom():RandomGenerator(){
    generator_ = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(generator_, time(NULL));
}


PnlRandom::PnlRandom(const PnlRandom & pnlRandom):RandomGenerator(){
    generator_ = pnl_rng_copy(pnlRandom.generator_);
}

PnlRandom::~PnlRandom(){
    pnl_rng_free(&generator_);
}


/* fill matrix with random values*/
void PnlRandom::fillMatrix(PnlMat *matrix, int nbOfRows, int nbOfCol){
    pnl_mat_rng_normal(matrix, nbOfRows, nbOfCol, generator_);
}
