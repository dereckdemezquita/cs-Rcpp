// Include the Rcpp.h header
#include <Rcpp.h>

// Use the Rcpp namespace
using namespace Rcpp;

// [[Rcpp::export]]
int the_answer() {
	// Return 42
	return 42;
}

/*** R
# Call the_answer() to check you get the right result
the_answer()
*/