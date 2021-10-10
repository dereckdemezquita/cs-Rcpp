#include <Rcpp.h>
using namespace Rcpp; 

// [[Rcpp::export]]
NumericVector na_meancf_cpp(NumericVector x) {
	double total_not_na = 0.0;
	double n_not_na = 0.0;
	NumericVector res = clone(x);
	
	int n = x.size();
	for(int i = 0; i < n; i++) {
		// If ith value of x is NA
		if(NumericVector::is_na(x[i])) {
			// Set the ith result to the total of non-missing values 
			// divided by the number of non-missing values
			res[i] = total_not_na / n_not_na;
		} else {
			// Add the ith value of x to the total of non-missing values
			total_not_na += x[i];
			// Add 1 to the number of missing values
			n_not_na++;
		}
	}
	return res;
}

/*** R
set.seed(42)
x <- rnorm(1e5)
x[sample(1e5, 100)] <- NA  
res <- microbenchmark::microbenchmark( 
	na_meancf_R(x), 
	na_meancf_cpp(x), 
	times = 5
)

ggplot2::autoplot(res)

remove(res)
*/
