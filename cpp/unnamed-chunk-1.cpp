#include <Rcpp.h>
using namespace Rcpp;

// From previous exercise; do not modify
// [[Rcpp::export]]
int choose_component(NumericVector weights, double total_weight) {
	double x = R::runif(0, total_weight);
	int j = 0;
	while(x >= weights[j]) {
		x -= weights[j];
		j++;
	}
	return j;
}

// [[Rcpp::export]]
NumericVector rmix(int n, NumericVector weights, NumericVector means, NumericVector sds) {
	// Check that weights and means have the same size
	int d = weights.size();
	if(means.size() != d) {
		stop("means size != weights size");
	}
	
	// Do the same for the weights and std devs
	if(sds.size() != d) {
		stop("sds size != weights size");
	}
	
	// Calculate the total weight
	double total_weight = sum(weights);
	
	// Create the output vector
	NumericVector res(n);
	
	// Fill the vector
	for(int i = 0; i < n; i++) {
		// Choose a component
		int j = choose_component(weights, total_weight);
		
		// Simulate from the chosen component
		res[i] = R::rnorm(means[j], sds[j]);
	}
	
	return res;
}

/*** R
weights <- c(0.3, 0.7)
means <- c(2, 4)
sds <- c(2, 4)
rmix(10, weights, means, sds)
*/
