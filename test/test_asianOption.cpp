#include <iostream>
#include "../src/AsianOption.hpp"

using namespace std;

int main()
{
    PnlVect * weights = pnl_vect_create_from_list(3, 0.3, 0.2, 0.5);
    AsianOption asianOption = AsianOption(4,2,3,10,weights);
    int nbUnderlyings = 3;
    PnlMat * past = pnl_mat_create(3, nbUnderlyings);
    pnl_mat_set(past, 0, 0, 10);
    pnl_mat_set(past, 0, 1, 11);
    pnl_mat_set(past, 0, 2, 12);
    pnl_mat_set(past, 1, 0, 9);
    pnl_mat_set(past, 1, 1, 10);
    pnl_mat_set(past, 1, 2, 11);
    pnl_mat_set(past, 2, 0, 11);
    pnl_mat_set(past, 2, 1, 9);
    pnl_mat_set(past, 2, 2, 10);
    double payOffReturned = asianOption.payoff(past);

    cout<<"The Payoff of the Asian Option has to be equal to 0.5 and is : " << payOffReturned;
    return 0;
}
