//#pragma once
//
//#include "pnl/pnl_random.h"
//#include "pnl/pnl_vector.h"
//#include "pnl/pnl_matrix.h"
//
///// \brief Classe Option abstraite
//class OptionAsian : public Option
//{
//public:
//    double T_; /// maturité
//    int nbTimeSteps_; /// nombre de pas de temps de discrétisation
//    int size_; /// dimension du modèle, redondant avec BlackScholesModel::size_
//    PnlVect *weights;
//    double strike;
//
//
//    /**
//     * Calcule la valeur du payoff sur la trajectoire
//     *
//     * @param[in] path est une matrice de taille (N+1) x d
//     * contenant une trajectoire du modèle telle que créée
//     * par la fonction asset.
//     * @return phi(trajectoire)
//     */
//    virtual double payoff(const PnlMat *path){
//    	PnlVect *vect ;
//    	pnl_mat_sum_vect(vect, path, 'r');
//    	return pnl_vect_scalar_prod(vect, weights)/(nbTimeSteps_+1)- strike;
//
//    }
//};
