#include <Rcpp.h>
using namespace Rcpp;

// Set the return type to a standard double vector
// [[Rcpp::export]]
std::vector<double> select_positive_values_std(NumericVector x) {
	int n = x.size();
	
	// Create positive_x, a standard double vector
	std::vector<double> positive_x(0);
	
	for(int i = 0; i < n; i++) {
		if(x[i] > 0) {
		// Append the ith element of x to positive_x
			positive_x.push_back(x[i]);
		}
	}
	return positive_x;
}

/*** R
set.seed(42)
x <- rnorm(1e6)
# Does it give the same answer as R?
all.equal(select_positive_values_std(x), x[x > 0])
# Which is faster?
res <- microbenchmark::microbenchmark(
	good_cpp = good_select_positive_values_cpp(x),
	std = select_positive_values_std(x),
	times = 3
)

ggplot2::autoplot(res)

remove(res)
*/
