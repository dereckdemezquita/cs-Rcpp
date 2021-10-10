#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector rollmean_cpp(NumericVector x, int window) {
	int n = x.size();
	
	// Set res as a NumericVector of NAs with length n
	NumericVector res(n, NumericVector::get_na());
	
	// Sum the first window worth of values of x
	double total = 0.0;
	for(int i = 0; i < window; i++) {
		total += x[i];
	}
	
	// Treat the first case seperately
	res[window - 1] = total / window;
	
	// Iteratively update the total and recalculate the mean 
	for(int i = window; i < n; i++) {
		// Remove the (i - window)th case, and add the ith case
		total += - x[i - window] + x[i];
		// Calculate the mean at the ith position
		res[i] = total / window;
	}
	
	return res;  
}

/*** R
# Compare rollmean2, rollmean3 and rollmean4   
set.seed(42)
x <- rnorm(1e4)
   
res <- microbenchmark::microbenchmark(
	rollmean1(x, 4),
	rollmean2(x, 4), 
	rollmean3(x, 4),
	rollmean_cpp(x, 4),
	times = 5
)

ggplot2::autoplot(res)

remove(res)
*/
