# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double sqrt_approx(double value, int n) {
	double x = 1;
	for(int i = 0; n > i; i++) {
		x = (x + value / x) / 2.0;
	}
	
	return x;
}

/*** r
sqrt_approx(2, 10)
*/
