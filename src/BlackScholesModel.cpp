#include "BlackScholesModel.hpp"
#include <stdexcept>
#include <iostream>


BlackScholesModel::BlackScholesModel()
{
    size_ = 0;
    r_ = 0;
    rho_ = 0;
    sigma_ = pnl_vect_new();
    spot_ = pnl_vect_new();
    trend_ = pnl_vect_new();
}

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot)
{
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(sigma_, sigma);
    spot_ = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(spot_, spot);
    trend_ = pnl_vect_create_from_scalar(size_, r);
}

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot, PnlVect * trend)
{
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(sigma_, sigma);
    spot_ = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(spot_, spot);
    trend_ = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(trend_, trend);

}

BlackScholesModel::BlackScholesModel(const BlackScholesModel & blackScholesModel)
{
    size_ = blackScholesModel.size_;
    r_ = blackScholesModel.r_;
    rho_ = blackScholesModel.rho_;
    sigma_ = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(sigma_, blackScholesModel.sigma_);
    spot_ = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(spot_, blackScholesModel.spot_);
    if (blackScholesModel.trend_->size != 0)
    {
        trend_ = pnl_vect_create_from_scalar(size_, 0);
        pnl_vect_clone(trend_, blackScholesModel.trend_);
    }
    else
    {
        trend_ = pnl_vect_new();
    }

}

BlackScholesModel::~BlackScholesModel()
{
	pnl_vect_free(&sigma_);
	pnl_vect_free(&spot_);
	pnl_vect_free(&trend_);
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, RandomGenerator *randomGenerator)
{
	double timeVariation = T/nbTimeSteps;
	PnlMat * corrMat = pnl_mat_create_from_scalar(size_, size_, rho_); // Create a PnlMat where all the elements = rho_
	// Now we change all diag elements to 1
	pnl_mat_set_diag(corrMat, 1, 0);

	pnl_mat_chol(corrMat);

	PnlMat * gaussMat = pnl_mat_create(nbTimeSteps, size_);
	randomGenerator->fillMatrix(gaussMat, nbTimeSteps, size_);

	PnlVect * vectTmp = pnl_vect_create(size_);
	PnlVect * previousSpots = pnl_vect_create(size_);
	pnl_vect_clone(previousSpots, spot_);
	PnlVect * lineGauss = pnl_vect_create(size_);


	// Start of trajectory generation
	pnl_mat_set_row(path, spot_, 0);
	PnlVect * trend = pnl_vect_create_from_scalar(size_, r_);
	for (int i=1; i<nbTimeSteps+1; i++)	{

		pnl_mat_get_row(lineGauss, gaussMat, i-1);

		computeVectMarket(vectTmp, size_, previousSpots, trend, sigma_, timeVariation, corrMat, lineGauss);

		pnl_mat_set_row(path, vectTmp, i);

		pnl_vect_clone(previousSpots, vectTmp);

	}
	//Free
	pnl_mat_free(&corrMat);
	pnl_mat_free(&gaussMat);
	pnl_vect_free(&vectTmp);
	pnl_vect_free(&previousSpots);
	pnl_vect_free(&lineGauss);
	pnl_vect_free(&trend);
}

void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, RandomGenerator *randomGenerator, const PnlMat *past)
{
    int sizePast = past->m;
    int numberOfExpectedElements = ceil(t*nbTimeSteps/T) + 1;
    if (numberOfExpectedElements != sizePast)
    {

        std::cerr<<"\nERROR: size of Past doesn't fit the time t\nSizePast = "<<sizePast<<"\tnumberOfExpectedElements = "<<numberOfExpectedElements<<"\n";
        throw;
    }

    double timeI1 = 0;
    double timeI = 0;
    double timeVariation = T/nbTimeSteps;
    PnlVect * previousSpots = pnl_vect_create(size_);
    PnlVect * vectTmp = pnl_vect_create_from_scalar(size_, 0);

    pnl_mat_set_row(path, spot_, 0); // Copy the initial spots

    // Start with i=1, because the initial spots have already been copied
    for (int i=1; i<sizePast-1; i++)
    {
        timeI += timeVariation;
        pnl_mat_get_row(vectTmp, past, i);
        pnl_mat_set_row(path, vectTmp, i);
    }
    // I copy the last row of Past, which represents the spots at t
    pnl_mat_get_row(vectTmp, past, sizePast-1);
    pnl_vect_clone(previousSpots, vectTmp);
    timeI1 = timeI + timeVariation;
    timeI = t;

    PnlMat * corrMat = pnl_mat_create_from_scalar(size_, size_, rho_); // Create a PnlMat where all the elements = rho_
    // Now we change all diag elements to 1

    pnl_mat_set_diag(corrMat, 1, 0);


    int result = pnl_mat_chol(corrMat);

    PnlMat * gaussMat = pnl_mat_create(nbTimeSteps-(sizePast-1)+1, size_);
    randomGenerator->fillMatrix(gaussMat, nbTimeSteps-(sizePast-1)+1 , size_);
	PnlVect * lineGauss = pnl_vect_create(size_);

	int beginning = (sizePast-1 > 1)? sizePast-1 : 1 ;
	PnlVect * trend = pnl_vect_create_from_scalar(size_, r_);

	for (int i=beginning; i<nbTimeSteps+1; i++)
	{
		timeVariation = timeI1 - timeI;
		pnl_mat_get_row(lineGauss, gaussMat, i-(sizePast-1));
		//computeVect(vectTmp, size_, previousSpots, r_, sigma_, timeVariation, corrMat, lineGauss);
		computeVectMarket(vectTmp, size_, previousSpots, trend, sigma_, timeVariation, corrMat, lineGauss);
		pnl_mat_set_row(path, vectTmp, i);
		pnl_vect_clone(previousSpots, vectTmp);
		timeI = timeI1;
		timeI1 = timeI1 + T/nbTimeSteps;
	}
	//Free
	pnl_vect_free(&previousSpots);
	pnl_vect_free(&vectTmp);
	pnl_mat_free(&corrMat);
	pnl_mat_free(&gaussMat);
	pnl_vect_free(&lineGauss);
	pnl_vect_free(&trend);
}

void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep)
{
    int index = 0;
    int sizeMat = path->m;
    double timeI = 0;
    PnlVect * vectTmp = pnl_vect_create(size_);

    // Copy the lines of path to shift_path where tI < t and without being outrange
    while (timeI < t && index<sizeMat)
    {
        pnl_mat_get_row(vectTmp, path, index);
        pnl_mat_set_row(shift_path, vectTmp, index);
        index += 1;
        timeI += timestep;
    }
    double valueBeforeShift = 0;

    // Shift the column for the underlying number d, and starting with the line index
    for(int i=index; i<sizeMat; i++)
    {
        pnl_mat_get_row(vectTmp, path, i);
        valueBeforeShift = GET(vectTmp, d);
        LET(vectTmp, d) = valueBeforeShift*(1+h);
        pnl_mat_set_row(shift_path, vectTmp, i);
    }
    //Free
    pnl_vect_free(&vectTmp);
}

void computeVectMarket(PnlVect *vect, int size, PnlVect * previousSpots, PnlVect * r, PnlVect *sigma, double variation, PnlMat * cholMat, PnlVect * gaussVect)
{
    PnlVect * lineChol = pnl_vect_create(size);
    double tmp = 0;
    double expResult = 0;
    double result = 0;
    double sigma_i = 0;
    double previousSpots_i = 0;
    PnlVect * cholMultGauss = pnl_mat_mult_vect(cholMat, gaussVect);
    double cholMultGauss_i = 0;
    for (int i=0; i<size; i++)
    {
        cholMultGauss_i = GET(cholMultGauss, i);

        //tmp = (r[i] - (sigma[i]*sigma[i]/2))*variation + sigma[i] * cholMultGauss_i * sqrt(variation);
        sigma_i = GET(sigma, i);
        tmp = (pnl_vect_get(r, i) - (sigma_i*sigma_i/2))*variation + sigma_i * cholMultGauss_i * sqrt(variation);

        expResult = exp(tmp);

        //result = previousSpots[i] * expResult;
        previousSpots_i = GET(previousSpots, i);
        result = previousSpots_i* expResult;

        //vect[i] = result;
        pnl_vect_set(vect, i, result);

    }
    //Free
    pnl_vect_free(&lineChol);
}


void BlackScholesModel::simul_market(PnlMat * path, double T,int nbSteps, RandomGenerator * randomGenerator)
{
    double timeVariation = T/nbSteps;
    PnlMat * corrMat = pnl_mat_create_from_scalar(size_, size_, rho_); // Create a PnlMat where all the elements = rho_
    // Now we change all diag elements to 1
    for(int i=0; i<size_; i++)
    {
        pnl_mat_set(corrMat, i, i, 1);
    }
    //pnl_mat_print(corrMat);
    pnl_mat_chol(corrMat);
    //pnl_mat_print(corrMat);

    PnlMat * gaussMat = pnl_mat_create_from_scalar(nbSteps, size_, 0);
    randomGenerator->fillMatrix(gaussMat, nbSteps, size_);

    PnlVect * vectTmp = pnl_vect_create_from_scalar(size_, 0);
    PnlVect * previousSpots = pnl_vect_create_from_scalar(size_, 0);
    pnl_vect_clone(previousSpots, spot_);
    PnlVect * lineGauss = pnl_vect_create_from_scalar(size_, 0);


    // Start of trajectory generation
    pnl_mat_set_row(path, spot_, 0);

    for (int i=1; i<nbSteps+1; i++)	{

        pnl_mat_get_row(lineGauss, gaussMat, i-1);

        computeVectMarket(vectTmp, size_, previousSpots, trend_, sigma_, timeVariation, corrMat, lineGauss);
        //pnl_mat_print(corrMat);

        pnl_mat_set_row(path, vectTmp, i);

        pnl_vect_clone(previousSpots, vectTmp);

    }
    //Free
    pnl_mat_free(&corrMat);
    pnl_mat_free(&gaussMat);
    pnl_vect_free(&vectTmp);
    pnl_vect_free(&previousSpots);
    pnl_vect_free(&lineGauss);

}


