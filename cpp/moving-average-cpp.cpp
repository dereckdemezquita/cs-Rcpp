#include <Rcpp.h>
using namespace Rcpp ;

// [[Rcpp::export]]
NumericVector ma_cpp( int n, double mu, NumericVector theta, double sd ){
	int q = theta.size(); 
	NumericVector x(n);
	
	// Generate the noise vector
	NumericVector eps = rnorm(n, 0.0, sd);
	
	// Loop from q to n
	for(int i = q; i < n; i++) {
		// Value is mean plus noise
		double value = mu + eps[i];
		// Loop from zero to q
		for(int j = 0; j < q; j++) {
			// Increase by the jth element of theta times
			// the "i minus j minus 1"th element of eps
			value += theta[j] * eps[i - j - 1];
		}
		// Set ith element of x to value
		x[i] = value;
	}
	return x;
}

/*** R
d <- data.frame(
	x = 1:50,
	y = ma_cpp(50, 10, c(1, -0.5), 1)
)

ggplot(d, aes(x, y)) +
	geom_line()

remove(d)
*/
