# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]

NumericVector ones(int n) {
	// Initialise numeric vector of size n
	NumericVector x(n);
	
	for(int i = 0; i < n; i++) {
		x[i] = 1;
	}
	return x;
}

/*** R
ones(10)
*/
