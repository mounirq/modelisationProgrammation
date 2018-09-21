#ifndef PROJETPRICERGIT_ABSTRACTPRICER_H
#define PROJETPRICERGIT_ABSTRACTPRICER_H


#include "BlackScholesModel.hpp"
#include "Option.hpp"

class AbstractPricer {

public:


    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    Option *opt_; /*! pointeur sur l'option */
    RandomGenerator *rng_; /*! pointeur sur le générateur */
    double fdStep_; /*! pas de différence finie */
    size_t nbSamples_; /*! nombre de tirages Monte Carlo */

    AbstractPricer();
//    AbstractPricer(char *fileName);
//    AbstractPricer(BlackScholesModel *model, Option *option, PnlRng *rng, double d, size_t i);

    virtual void price(double &prix, double &ic);

    /**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    virtual void price(const PnlMat *past, double t, double &prix, double &ic);

    /**
     * Calcule le delta de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * de confiance sur le calcul du delta
     */
    virtual void delta(const PnlMat *past, double t, PnlVect *delta);

    /**
     * Calcule le P&L a partir d'une trajectoire de marché donnée
     *
     * @param[in] market_trajectory contient la trajectoire du sous-jacent
     * sur une grille de temps 0 à T
     * @param[out] p_and_l  contient l'erreur de couverture
     */
    void profits_and_losses(const PnlMat *market_trajectory, double &p_and_l);

    virtual ~AbstractPricer();
};


#endif //PROJETPRICERGIT_ABSTRACTPRICER_H
