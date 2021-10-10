#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector arma(int n, double mu, NumericVector phi, NumericVector theta, double sd) {
	int p = phi.size();
	int q = theta.size();   
	NumericVector x(n);
	
	// Generate the noise vector
	NumericVector eps = rnorm(n, 0.0, sd);
	
	// Start at the max of p and q plus 1
	int start = std::max(p, q) + 1;
	
	// Loop i from start to n
	for(int i = start; i < n; i++) {
		// Value is mean plus noise
		double value = mu + eps[i];
		
		// The MA(q) part
		for(int j = 0; j < q; j++) {
			// Increase by the jth element of theta times
			// the "i minus j minus 1"th element of eps
			value += theta[j] * eps[i - j - 1];
		}
		
		// The AR(p) part
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
	y = arma(50, 10, c(1, -0.5), c(1, -0.5), 1)
)

ggplot(d, aes(x, y)) +
	geom_line()

remove(d)
*/
