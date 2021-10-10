#include <Rcpp.h>
using namespace Rcpp ;

// [[Rcpp::export]]
double absolute(double x) {
	// Test for x greater than zero
	if(x > 0) {
		// Return x
		return x;
		// Otherwise
	} else {
		// Return negative x
		return -x;
	}
}

/*** R  
absolute(pi)
absolute(-3)
*/