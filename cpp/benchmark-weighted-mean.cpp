#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double weighted_mean_cpp(NumericVector x, NumericVector w) {
	// Initialise these to zero
	double total_w = 0;
	double total_xw = 0;
	
	// Set n to the size of x
	int n = x.size();
	
	for(int i = 0; i < n; i++) {
		// Add the ith weight
		total_w += w[i];
		
		// Add the ith data value times the ith weight
		total_xw += x[i] * w[i];
	}
	
	return total_xw / total_w;
}

/***r
x <- c(0, 1, 3, 6, 2, 7, 13, 20, 12, 21, 11)
w <- 1 / seq_along(x)
weighted_mean_cpp(x, w)

# Does the function give the same results as R's weighted.mean() function?
identical(weighted_mean_cpp(x, w), weighted.mean(x, w))

res <- microbenchmark::microbenchmark(
	weighted_mean_cpp = weighted_mean_cpp(x, w),
	weighted.mean_R = weighted.mean(x, w),
	times = 3
)

ggplot2::autoplot(res)

remove(res)
*/
