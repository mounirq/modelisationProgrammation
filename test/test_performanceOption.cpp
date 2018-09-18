#include <iostream>
#include "../src/PerformanceOption.hpp"

using namespace std;

int main()
{
    PnlVect * weights = pnl_vect_create_from_list(1, 1);
    PerformanceOption optionPerformance  = PerformanceOption(2, 2, 2, 12, weights);
    int nbUnderlyings = 2;
    PnlMat * past = pnl_mat_create(5, nbUnderlyings);
    pnl_mat_set(past, 0, 0, 2);
    pnl_mat_set(past, 0, 1, 2);
    for(int i = 1; i<4; ++i)
    {
    	for (int j =0; j<2; ++j)
    	{
    		pnl_mat_set(past, i, j, 1);
    	}
    }
    pnl_mat_set(past, 4, 0, 2);
    pnl_mat_set(past, 4, 1, 2);
    double payOffReturned = optionPerformance.payoff(past);
    cout<<"The Payoff of the Performance Option has to be equal to 1.5 and is : " << payOffReturned;
    return 0;
}
