//#pragma once
//
//#include "pnl/pnl_random.h"
//#include "pnl/pnl_vector.h"
//#include "pnl/pnl_matrix.h"
//
///// \brief Classe Option abstraite
//class OptionPerformance : public Option
//{
//public:
//    double T_; /// maturité
//    int nbTimeSteps_; /// nombre de pas de temps de discrétisation
//    int size_; /// dimension du modèle, redondant avec BlackScholesModel::size_
//    PnlVect *weights;
//    double strike;
//
//    Option
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
//    	PnlVect *tmp1;
//    	PnlVect *tmp2;
//    	vect = pnl_mat_mult_vect(path, weights);
//    	tmp1 = pnl_vect_create_subvect(vect, 1, nbTimeSteps_+1);
//    	pnl_vect_extract_subvect(tmp2, vect, 0,nbTimeSteps_);
//    	pnl_vect_div_vect_term(tmp1, tmp2);
//    	pnl_vect_minus_scalar(tmp1, 1);
//    	return pnl_vect_sum(tmp1)+1;
//
//    }
//};
