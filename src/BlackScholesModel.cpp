#include "BlackScholesModel.hpp"


BlackScholesModel::BlackScholesModel()
{
	size_ = 0;
	r_ = 0;
	rho_ = 0;
	sigma_ = pnl_vect_new();
	spot_ = pnl_vect_new();
}

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot)
{
	size_ = size;
	r_ = r;
	rho_ = rho;
	sigma_ = pnl_vect_copy(sigma);
	spot_ = pnl_vect_copy(spot);
}

BlackScholesModel::BlackScholesModel(const BlackScholesModel & blackScholesModel)
{
	size_ = blackScholesModel.size_;
	r_ = blackScholesModel.r_;
	rho_ = blackScholesModel.rho_;
	sigma_ = pnl_vect_copy(blackScholesModel.sigma_);
	spot_ = pnl_vect_copy(blackScholesModel.spot_);
}

BlackScholesModel::~BlackScholesModel()
{
	delete sigma_;
	delete spot_;
}

PnlVect * computeVect(int size, PnlVect * previousSpots, double r, PnlVect *sigma, double variation, PnlMat * cholMat, PnlVect * gaussVect)
{

	//double scalarCholGauss = pnl_vect_scalar_prod(upperChol, gaussVect);
	PnlVect * vect = pnl_vect_create(size);
	PnlVect * lineChol = pnl_vect_create(size);
	double scalarCholGauss = 0;
	double tmp = 0;
	double expResult = 0;
	double result = 0;
	for (int i=0; i<size; i++)
	{
		pnl_mat_get_row(lineChol, cholMat, i);
		scalarCholGauss = pnl_vect_scalar_prod(lineChol, gaussVect);

		//tmp = (r - (sigma[i]*sigma[i]/2))*variation + sigma[i] * scalarCholGauss * sqrt(variation);
		tmp = (r - (pnl_vect_get(sigma, i)*pnl_vect_get(sigma, i)/2))*variation + pnl_vect_get(sigma, i) * scalarCholGauss * sqrt(variation);

		expResult = exp(tmp);

		//result = previousSpots[i] * expResult;
		result = pnl_vect_get(previousSpots, i) * expResult;

		//vect[i] = result;
		pnl_vect_set(vect, i, result);

	}
	return vect;

}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng)
{
	double timeVariation = T/nbTimeSteps;
	PnlMat * corrMat = pnl_mat_create_from_scalar(size_, size_, rho_); // Create a PnlMat where all the elements = rho_
	// Now we change all diag elements to 1
	for(int i=0; i<size_; i++)
	{
		pnl_mat_set(corrMat, i, i, 1);
	}
	pnl_mat_print(corrMat);
	int result = pnl_mat_chol(corrMat);
	pnl_mat_print(corrMat);

	PnlMat * gaussMat = pnl_mat_create(nbTimeSteps, size_);
	pnl_mat_rng_normal(gaussMat, nbTimeSteps , size_, rng);

	PnlVect * vectTmp = pnl_vect_create(size_);
	PnlVect * previousSpots = pnl_vect_copy(spot_);
	PnlVect * lineGauss = pnl_vect_create(size_);


	// Start of trajectory generation
	pnl_mat_set_row(path, spot_, 0);

	for (int i=1; i<nbTimeSteps+1; i++)	{

		pnl_mat_get_row(lineGauss, gaussMat, i-1);

		vectTmp = computeVect(size_, previousSpots, r_, sigma_, timeVariation, corrMat, lineGauss);
		//pnl_mat_print(corrMat);

		pnl_mat_set_row(path, vectTmp, i);

		previousSpots = pnl_vect_copy(vectTmp);

	}
	//pnl_vect_print(previousSpots);
	//pnl_mat_print(path);

}

