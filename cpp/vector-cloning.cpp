#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List change_negatives_to_zero(NumericVector the_original) {
	// Set the copy to the original
	NumericVector the_copy = the_original;
	int n = the_original.size();
	for(int i = 0; i < n; i++) {
		if(the_copy[i] < 0) the_copy[i] = 0;
	}
	return List::create(_["the_original"] = the_original, _["the_copy"] = the_copy);
}

// [[Rcpp::export]]
List change_negatives_to_zero_with_cloning(NumericVector the_original) {
// Clone the original to make the copy
NumericVector the_copy = clone(the_original);
	int n = the_original.size();
	for(int i = 0; i < n; i++) {
		if(the_copy[i] < 0) the_copy[i] = 0;
	}
	return List::create(_["the_original"] = the_original, _["the_copy"] = the_copy);
}

/*** R
x <- c(0, -4, 1, -2, 2, 4, -3, -1, 3)
change_negatives_to_zero(x)
# Need to define x again because it's changed now
x <- c(0, -4, 1, -2, 2, 4, -3, -1, 3)
change_negatives_to_zero_with_cloning(x)
*/
