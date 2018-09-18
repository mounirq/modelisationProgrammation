#include <iostream>
#include "../src/BasketOption.hpp"

using namespace std;

int main()
{
    PnlVect * weights = pnl_vect_create_from_list(2, 0.3, 0.7);
    BasketOption basketOption = BasketOption(2, 2, 2, 12, weights);
    int nbUnderlyings = 2;
    PnlMat * past = pnl_mat_create(3, nbUnderlyings);
    pnl_mat_set(past, 0, 0, 10);
    pnl_mat_set(past, 0, 1, 15);
    pnl_mat_set(past, 1, 0, 10.292682);
    pnl_mat_set(past, 1, 1, 15.169634);
    pnl_mat_set(past, 2, 0, 10.263704);
    pnl_mat_set(past, 2, 1, 15.039295);
    double payOffReturned = basketOption.payoff(past);

    cout<<"The Payoff of the Basket Option has to be equal to 1.60662 and is : " << payOffReturned;
    return 0;
}
