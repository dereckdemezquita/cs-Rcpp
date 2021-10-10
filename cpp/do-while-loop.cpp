#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double sqrt_approx(double value, double threshold) {
	double x = 1.0;
	double previous = x;
	bool is_good_enough = false;
	
	// Initiate do while loop
	do {
		  previous = x;
		  x = (x + value / x) / 2.0;
		  is_good_enough = fabs(x - previous) < threshold;
	// Specify while condition
	} while(!is_good_enough);
	
	return x;
}

/*** R
sqrt_approx(2, 0.00001)
*/
