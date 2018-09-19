#ifndef OPTION_H
#define OPTION_H
#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"


class Option
{
public:
    double T_; /// Maturity
    int nbTimeSteps_; /// Number of time steps discretization
    int size_; /// Model dimension, the same with BlackScholesModel::size_
    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat *path) = 0;

    Option();

    Option(double T_, int nbTimeSteps_, int size_);

    Option(const Option & option);

    virtual ~Option();
};
#endif
