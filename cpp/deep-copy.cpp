# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector positives(NumericVector x) {
	NumericVector y = clone(x);
	
	return y;
}

/*** R
positives(1:10)
*/
