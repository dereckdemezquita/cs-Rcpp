#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double first_plus_last(NumericVector x) {
	// The size of x
	int n = x.size();
	// The first element of x
	double first = x[0];
	// The last element of x
	double last = x[n-1];
	return first + last;
}

/*** R
x <- c(6, 28, 496, 8128)

first_plus_last(x)
# Does the function give the same answer as R?
all.equal(first_plus_last(x), x[1] + x[4], 6 + 8128)
*/
