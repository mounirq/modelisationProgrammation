#include "MonteCarlo.hpp"
#include "PerformanceOption.hpp"
#include "BasketOption.hpp"
#include "AsianOption.hpp"

MonteCarlo::MonteCarlo(char *fileName)
{
    Param *P = new Parser(fileName);

    size_t size1;
    int size;
    double r;
    double rho;
    PnlVect *sigma, *spot;
    fdStep_ = 0.1;

    P->extract("sample number", this->nbSamples_);
    P->extract("option size", size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("volatility", sigma, size);
    P->extract("spot", spot, size);
    this->mod_ = new BlackScholesModel(size, r, rho, sigma, spot);

    double T;
    int nbTimeSteps;
    PnlVect *weights;
    std::string optionType;
    P->extract("option type", optionType);

    P->extract("maturity", T);
    P->extract("timestep number", nbTimeSteps);
    P->extract("payoff coefficients", weights, size);

    rng_ = new PnlRandom();

    if (optionType.compare("performance") == 0 )
    {
        this->opt_ = new PerformanceOption(T, nbTimeSteps, size, weights);
    }
    else {
        double strike;
        P->extract("strike", strike);
        if (optionType.compare("basket") == 0 )
        {
            this->opt_ = new BasketOption(T, nbTimeSteps, size, strike, weights);
        }
        else if (optionType.compare("asian") == 0 )
        {
            this->opt_ = new AsianOption(T, nbTimeSteps, size, strike, weights);
        }
        else {
            std::cerr << "Unkonwn option type" << std::endl;
        }
    }
    pnl_vect_free(&weights);
    pnl_vect_free(&sigma);
    pnl_vect_free(&spot);
    delete P;
}

MonteCarlo::MonteCarlo(BlackScholesModel *mod, Option *opt, RandomGenerator *rng, double fdStep, size_t nbSamples)
{
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
}

MonteCarlo::~MonteCarlo() {
    delete mod_;
    delete opt_;
    delete rng_;
}

void MonteCarlo::price(double &prix, double &ic)
{
    PnlMat *path = pnl_mat_create_from_scalar(opt_->nbTimeSteps_+1,mod_->size_, 0);

    double sum = 0;
    double icSum = 0;
    double payoffTmp = 0;
    double maturity = opt_->T_;
    double nbTimeSteps = opt_->nbTimeSteps_;
    for (int i=0 ; i<nbSamples_ ; i++)
    {
        mod_->asset(path, maturity, nbTimeSteps, rng_);
        payoffTmp = opt_->payoff(path);
        sum += payoffTmp;
        icSum += payoffTmp * payoffTmp;
    }
    prix = (exp(-mod_->r_ * opt_->T_)/nbSamples_) * sum;

    double varApprochee = exp(- 2 * mod_->r_ * opt_->T_) * ( ((1.0/nbSamples_) * icSum) - (((1.0/nbSamples_) * sum) * ((1.0/nbSamples_) * sum)) );

    ic = 2 * 1.96 * sqrt(varApprochee/nbSamples_);

    pnl_mat_free(&path);

}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic)
{
    int nbActifs = mod_->size_;
    PnlMat *path = pnl_mat_create_from_scalar(opt_->nbTimeSteps_+1,nbActifs, 0);

    double sum = 0;
    double icSum = 0;
    double payoffTmp = 0;

    for (int i=0 ; i<nbSamples_ ; i++)
    {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        payoffTmp = opt_->payoff(path);
        sum += payoffTmp;
        icSum = payoffTmp * payoffTmp;
    }
    prix = (exp(-mod_->r_ * (opt_->T_-t))/nbSamples_) * sum;

    double varApprochee = exp(- 2 * mod_->r_ * (opt_->T_ - t)) * ( ((1.0/nbSamples_) * icSum) - ((1.0/nbSamples_) * sum) * ((1.0/nbSamples_) * sum) );
    ic = 2 * 1.96 * sqrt(varApprochee/nbSamples_);

    pnl_mat_free(&path);
}

void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *icDelta) {
    int nbActifs = mod_->size_;
    PnlMat *shift_plus_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
    PnlMat *shift_moins_path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
    PnlMat *path = pnl_mat_create(opt_->nbTimeSteps_ + 1, nbActifs);
    PnlVect *vect_sum = pnl_vect_create_from_scalar(nbActifs, 0);
    PnlVect *sum = pnl_vect_create_from_scalar(nbActifs, 0);
    PnlVect *icSum = pnl_vect_create_from_scalar(nbActifs, 0);

    double deltaAttendu = 0;

    double timeStep = opt_->T_ / opt_->nbTimeSteps_;

    for (int j = 0; j < nbSamples_; j++) {

        if (t == 0) {
            mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        } else {
            mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        }

        double payoffShiftPlus = 0;
        double payoffShiftMoins = 0;
        for (int d = 0; d < nbActifs; d++) {

            mod_->shiftAsset(shift_plus_path, path, d, fdStep_, t, timeStep);

            mod_->shiftAsset(shift_moins_path, path, d, -fdStep_, t, timeStep);

            payoffShiftPlus = opt_->payoff(shift_plus_path);

            payoffShiftMoins = opt_->payoff(shift_moins_path);

            pnl_vect_set(sum, d, GET(sum,d) + payoffShiftPlus - payoffShiftMoins);

            pnl_vect_set(icSum, d, GET(icSum,d) + (payoffShiftPlus - payoffShiftMoins) * (payoffShiftPlus - payoffShiftMoins));

            pnl_vect_set(vect_sum, d, GET(vect_sum, d) + payoffShiftPlus - payoffShiftMoins);
        }
    }

    double tmpValueForIC = 0;
    int sizePast = past->m;

    double spot_d = 0;
    double vect_sum_d = 0;
    double icsum_d = 0;
    for (int d = 0; d < nbActifs; d++) {
        vect_sum_d = GET(vect_sum, d);
        if (t == 0) {
            spot_d = GET(mod_->spot_, d);
            tmpValueForIC = exp(- 2 * mod_->r_ * (opt_->T_ - t))/((2*fdStep_*spot_d)*(2*fdStep_*spot_d));

            deltaAttendu = vect_sum_d * exp(-mod_->r_ * (opt_->T_ - t))/(2.0 * nbSamples_ * MGET(path, 0, d)* fdStep_);
            pnl_vect_set(delta, d, deltaAttendu);
        } else {
            tmpValueForIC = exp(- 2 * mod_->r_ * (opt_->T_ - t))/((2*fdStep_*MGET(past,sizePast-1,d))*(2*fdStep_*MGET(past,sizePast-1,d)));
            pnl_vect_set(delta, d, vect_sum_d * exp(-mod_->r_ * (opt_->T_ - t))/(2.0 * nbSamples_ * MGET(past, past->m - 1, d) * fdStep_));
        }
        icsum_d = GET(icSum, d);
        pnl_vect_set(icDelta, d, 1.96 * sqrt((tmpValueForIC * (1.0*icsum_d/nbSamples_ - (1.0*vect_sum_d/nbSamples_)*(1.0*vect_sum_d/nbSamples_) ))/nbSamples_ ));

    }
    pnl_mat_free(&shift_plus_path);
    pnl_mat_free(&shift_moins_path);
    pnl_mat_free(&path);
    pnl_vect_free(&vect_sum);
    pnl_vect_free(&sum);
    pnl_vect_free(&icSum);
}

void MonteCarlo::profits_and_losses(const PnlMat *market_trajectory, double &p_and_l)
{
    int H = market_trajectory->m;
    int nbAssets = market_trajectory->n;

    PnlVect *vect_V = pnl_vect_create_from_scalar(H, 0);

    PnlVect *spots = pnl_vect_create_from_scalar(nbAssets, 0);
    pnl_vect_clone(spots, mod_->spot_);

    PnlMat *past = pnl_mat_create_from_scalar(opt_->nbTimeSteps_ + 1, nbAssets, 0);
    PnlMat *sub_past = pnl_mat_new();

    PnlVect *ics = pnl_vect_create_from_scalar(opt_->size_, 0); //pour les deltas

    PnlVect *diff_delta = pnl_vect_create_from_scalar(opt_->size_, 0);
    PnlVect *prev_delta = pnl_vect_create_from_scalar(nbAssets, 0);
    PnlVect *deltas = pnl_vect_create_from_scalar(nbAssets, 0);

    PnlVect *tmp_row = pnl_vect_create_from_scalar(nbAssets, 0);

    double prix = 0;
    double ic = 0;
    double v = 0;
    price(prix, ic);
    double prix0 = prix;
    delta(past, 0, deltas, ics); //a t=0 past n'est pas utilisé dans le calcul des deltas

    //calcul de V(0)
    v = prix - pnl_vect_scalar_prod(deltas, spots);
    pnl_vect_set(vect_V, 0, v);

    double step_for_delta = opt_->T_ / H;  // T/H
    double actualisationFactor = exp(mod_->r_ * step_for_delta);

    int step_for_payoff = H/opt_->nbTimeSteps_; // H/N

    double nbOfSpotsNeeded = 0;

    pnl_mat_get_row(tmp_row, market_trajectory, 0);
    pnl_mat_set_row(past, tmp_row, 0);

    double index_row_to_fill = 1;

    for (int i = 1; i<H; i++){

//        std::cout<< "Le t est = " << i << std::endl;
        //Mise à jour de la matrice past
        if (i%step_for_payoff == 0){
            pnl_mat_get_row(tmp_row, market_trajectory, step_for_payoff * index_row_to_fill);
            pnl_mat_set_row(past, tmp_row, index_row_to_fill);
            index_row_to_fill++;
        }
        else {
            pnl_mat_get_row(tmp_row, market_trajectory, i);
            pnl_mat_set_row(past, tmp_row, index_row_to_fill);
        }

        nbOfSpotsNeeded = ceil(((double)i)/(double)step_for_payoff) + 1;
        pnl_mat_extract_subblock(sub_past, past, 0, nbOfSpotsNeeded, 0, nbAssets);
//        std::cout<< "Le sub_past est = " << std::endl;
//        pnl_mat_print(sub_past);

        // diff_delta = delta(i) - delta(i-1)
        pnl_vect_clone(prev_delta, deltas);
        delta(sub_past, i*step_for_delta, deltas, ics);
        pnl_vect_clone(diff_delta, deltas);
        pnl_vect_minus_vect(diff_delta, prev_delta);

        //spots = S(tho_i)
        pnl_mat_get_row(spots, market_trajectory, i);

        // v = V(i-1) * exp(rT/H) - (delta(i) - delta(i-1)) * S(tho_i)
        v = GET(vect_V, i-1) * actualisationFactor - pnl_vect_scalar_prod(diff_delta, spots);
        pnl_vect_set(vect_V, i, v);
    }

//    std::cout<< "Le past est = " << std::endl;
//    pnl_mat_print(past);

    // A la sortie de la boucle : deltas = delta(H) et spots = S(tho_H)
    p_and_l = GET(vect_V, H-1) + pnl_vect_scalar_prod(deltas, spots) - opt_->payoff(past);

    std::cout<< "Le PL/prix0 = " << p_and_l/prix0 << std::endl;

    //Free
    pnl_vect_free(&vect_V);
    pnl_vect_free(&spots);
    pnl_vect_free(&ics);
    pnl_vect_free(&diff_delta);
    pnl_vect_free(&prev_delta);
    pnl_vect_free(&deltas);
    pnl_vect_free(&tmp_row);
    pnl_mat_free(&sub_past);
    pnl_mat_free(&past);
}
