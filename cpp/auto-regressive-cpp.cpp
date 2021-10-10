#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector ar_cpp(int n, double c, NumericVector phi, double eps) {
	int p = phi.size();  
	NumericVector x(n);
	
	// Loop from p to n
	for(int i = p; i < n; i++) {
		// Generate a random number from the normal distribution
		double value = R::rnorm(c, eps);
		// Loop from zero to p
		for(int j = 0; j < p; j++) {
			// Increase by the jth element of phi times 
			// the "i minus j minus 1"th element of x
			value += phi[j] * x[i - j - 1];
		}
		x[i] = value;
	}
	return x;
}

/*** R
d <- data.frame(
	x = 1:50,
	y = ar_cpp(50, 10, c(1, -0.5), 1)
)

ggplot(d, aes(x, y)) +
	geom_line()

remove(d)
*/
