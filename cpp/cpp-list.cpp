#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List create_vectors() {
	// Create an unnamed character vector
	CharacterVector polygons = CharacterVector::create("triangle", "square", "pentagon");
	// Create a named integer vector
	IntegerVector mersenne_primes = IntegerVector::create(_["first"] = 3, _["second"] = 7, _["third"] = 31);
	// Create a named list
	List both = List::create(_["polygons"] = polygons, _["mersenne_primes"] = mersenne_primes);
	return both;
}

/*** R
create_vectors()
*/
