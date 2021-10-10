#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector bad_select_positive_values_cpp(NumericVector x) {
	NumericVector positive_x(0);
	for(int i = 0; i < x.size(); i++) {
		if(x[i] > 0) {
			positive_x.push_back(x[i]);
		}
	}
	return positive_x;
}

// [[Rcpp::export]]
NumericVector good_select_positive_values_cpp(NumericVector x) {
	int n_elements = x.size();
	int n_positive_elements = 0;
	
	// Calculate the size of the output
	for(int i = 0; i < n_elements; i++) {
		// If the ith element of x is positive
		if(x[i] > 0) {
			// Add 1 to n_positive_elements
			n_positive_elements++;
		}
	}
	
	// Allocate a vector of size n_positive_elements
	NumericVector positive_x(n_positive_elements);
	
	// Fill the vector
	int j = 0;
	for(int i = 0; i < n_elements; i++) {
		// If the ith element of x is positive
		if(x[i] > 0) {
			// Set the jth element of positive_x to the ith element of x
			positive_x[j] = x[i];
			// Add 1 to j
			j++;
		}
	}
	return positive_x;
}

/*** R
set.seed(42)
x <- rnorm(1e4)
# Does it give the same answer as R?
all.equal(good_select_positive_values_cpp(x), x[x > 0])
# Which is faster?
res <- microbenchmark::microbenchmark(
	bad_cpp = bad_select_positive_values_cpp(x),
	good_cpp = good_select_positive_values_cpp(x)
)

ggplot2::autoplot(res)

remove(res)
*/
