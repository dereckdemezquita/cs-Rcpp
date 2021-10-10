#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
int choose_component(NumericVector weights, double total_weight) {
	// Generate a uniform random number from 0 to total_weight
	double x = R::runif(0, total_weight);
	
	// Remove the jth weight from x until x is small enough
	int j = 0;
	while(x >= weights[j]) {
		// Subtract jth element of weights from x
		x -= weights[j];
		j++;
	}
	
	return j;
}

/*** R
weights <- c(0.3, 0.7)
# Randomly choose a component 5 times
replicate(5, choose_component(weights, sum(weights)))
*/
