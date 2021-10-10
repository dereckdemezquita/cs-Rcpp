#include <Rcpp.h>
using namespace Rcpp; 

// Make square() accept and return a double
double square(double x) {
	// Return x times x
	return x * x ;
}

// [[Rcpp::export]]
double dist(double x, double y) {
	// Change this to use square()
	return sqrt(square(x) + square(y));
}