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
