#include <Rcpp.h>
using namespace Rcpp;

// Set the return type to IntegerVector
// [[Rcpp::export]]
IntegerVector seq_cpp(int lo, int hi) {
	int n = hi - lo + 1;
	 
	// Create a new integer vector, sequence, of size n
	IntegerVector sequence(n);
	 
	for(int i = 0; i < n; i++) {
		// Set the ith element of sequence to lo plus i
		sequence[i] = lo + i;
	}
	
	return sequence;
}

/*** R
lo <- -2
hi <- 5
seq_cpp(lo, hi)
# Does it give the same answer as R's seq() function?
identical(seq_cpp(lo, hi), seq(lo, hi))
*/
