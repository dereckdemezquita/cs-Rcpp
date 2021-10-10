# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector named_vector() {
	return NumericVector::create(_["a"] = 1, _["b"] = 2, 5, _["d"] = 10);
};

/*** R
named_vector()
*/
