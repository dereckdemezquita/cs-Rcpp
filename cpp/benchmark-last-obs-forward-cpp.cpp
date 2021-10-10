#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector na_locf_cpp(NumericVector x) {
	// Initialise to NA
	double current = NumericVector::get_na();
	
	int n = x.size();
	NumericVector res = clone(x);
	for(int i = 0; i < n; i++) {
		// If ith value of x is NA
		if(NumericVector::is_na(x[i])) {
			// Set ith result as current
			res[i] = current;
		} else {
			// Set current as ith value of x
			current = res[i];
		}
	}
	return res;
}

/*** R
set.seed(42)
x <- rnorm(1e5)
# Sprinkle some NA into x
x[sample(1e5, 100)] <- NA
res <- microbenchmark::microbenchmark(
	na_locf_R(x),
	na_locf_cpp(x),
	times = 5
)

ggplot2::autoplot(res)

remove(res)
*/
