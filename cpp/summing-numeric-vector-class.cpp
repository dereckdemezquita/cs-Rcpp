# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double sum_cpp(NumericVector x) {
	int n = x.size();
	double result = 0;
	
	for(int i = 0; i < n; i++) {
		result = x[i] + result;
	}
	
	return result;
}

/*** R
set.seed(42)
x <- rnorm(1e6)

sum_cpp(x)
# Does the function give the same answer as R's sum() function?
identical(sum_cpp(x), sum(x))
*/
