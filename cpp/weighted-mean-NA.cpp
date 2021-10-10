#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double weighted_mean_cpp(NumericVector x, NumericVector w) {
	double total_w = 0;
	double total_xw = 0;
	
	int n = x.size();
	
	for(int i = 0; i < n; i++) {
		// If the ith element of x or w is NA then return NA
		if(NumericVector::is_na(x[i]) || NumericVector::is_na(w[i])) {
			return NumericVector::get_na();
		}
		
		total_w += w[i];
		total_xw += x[i] * w[i];
	}
	
	return total_xw / total_w;
}

/*** R 
x <- c(0, 1, 3, 6, 2, 7, 13, NA, 12, 21, 11)
w <- 1 / seq_along(x)
weighted_mean_cpp(x, w)
*/
