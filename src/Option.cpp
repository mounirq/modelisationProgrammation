#include "Option.hpp"

Option::~Option(){}

Option::Option(double T, int nbTimeSteps, int size){
	T_ = T;
	nbTimeSteps_ = nbTimeSteps;
	size_=size;
}

Option::Option(const Option & option){
	T_ = option.T_;
	nbTimeSteps_ = option.nbTimeSteps_;
		size_= option.size_;
}