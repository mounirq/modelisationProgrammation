#ifndef OPTION_H
#define OPTION_H
#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
public:
    double T_; /// maturité
    int nbTimeSteps_; /// nombre de pas de temps de discrétisation
    int size_; /// dimension du modèle, redondant avec BlackScholesModel::size_
    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat *path) = 0;

    /* Constructeur par défaut
     * */
    Option();

    /* Constructeur avec paramètres
    * */
    Option(double T_, int nbTimeSteps_, int size_);

    /* Constructeur par copie
    * */
    Option(const Option & option);



    /* Destructeur abstrait */
    virtual ~Option();
};
#endif
