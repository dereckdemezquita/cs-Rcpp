# Introduction to `Rcpp` <img src="./figures/project-logo.png" width="100" align="right">

Course notes from datacamp course: [optimising R code with Rcpp](https://learn.datacamp.com/courses/optimizing-r-code-with-rcpp). 

## Interactive eval `C++`

One can mimic R's interactivity with `C++` using the `evalCpp()` function.


```r
Rcpp::evalCpp("40 + 2")
```

```
## [1] 42
```

```r
Rcpp::evalCpp("std::numeric_limits<int>::max()") # get the biggest representable 32 bit signed int
```

```
## [1] 2147483647
```

## Types in `C++` vs `R`

In `R` most numbers are of type `double` ie. floating point numbers. There are many number types in C++. We will only use `double` and `int` as those are the only number types ones used in `R`. Below I show you the number types in `R` vs `C++`.


```r
storage.mode(42)
```

```
## [1] "double"
```

```r
storage.mode(42L)
```

```
## [1] "integer"
```

```r
storage.mode(Rcpp::evalCpp("42.0")) # .0 forces to double
```

```
## [1] "double"
```

```r
storage.mode(Rcpp::evalCpp("42")) # literal integers are int
```

```
## [1] "integer"
```

Cast to a `double.` Beware of `integer` division in `C++` does not return a double, no warning is provided and you get only an integer back. As you can see below 3.25 is 3.25 however the `C++` equivalent returns 3. Set the return type to `(double)` for a more precise result.


```r
13 / 4
```

```
## [1] 3.25
```

```r
Rcpp::evalCpp("13 / 4")
```

```
## [1] 3
```

```r
Rcpp::evalCpp("(double) 13 / 4"); storage.mode(Rcpp::evalCpp("(double) 13 / 4"))
```

```
## [1] 3.25
```

```
## [1] "double"
```

## Benchmarking

We will be using `microbenchmark` package throughout along with the `ggplot2::autoplot()` function to visualise performance gains. Here I build a little function that sums all values in an array, let's compare it to the `R`'s built in `sum()` function.


```r
x <- runif(100000L) * 100

sum_loop <- function(x) {
	accum <- 0
	for (i in 1:length(x)) {
		accum <- accum + x[i]
	}
	return(accum)
}

all.equal(sum_loop(x), sum(x))
```

```
## [1] TRUE
```

```r
res <- microbenchmark::microbenchmark(
	sum_loop(x),
	sum(x),
	times = 3
)
```

```r
ggplot2::autoplot(res)
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmarking-plot-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

## Be explicit in `C++`

Here I show you that even though doing two very similar operations `2 + 2` you don't get back the exact same result. You have to declare the type first! Be explicit.


```r
x <- Rcpp::evalCpp("2 + 2")
y <- 2 + 2

storage.mode(x); storage.mode(y)
```

```
## [1] "integer"
```

```
## [1] "double"
```

Let's see if we can get back the same type by casting before the operation.


```r
storage.mode(Rcpp::evalCpp("(double) 2 + 2"))
```

```
## [1] "double"
```

Again here are some more examples how `C++` might not return what you expect.


```r
Rcpp::evalCpp("17 / 2")
```

```
## [1] 8
```

```r
Rcpp::evalCpp("(double) 17 / 2")
```

```
## [1] 8.5
```

```r
Rcpp::evalCpp("(int) 56.3")
```

```
## [1] 56
```


## Defining a `C++` function

In `C++` things are all statically types. You must declare the type of the arguments of a function and the type of the object returned. The return type is declared before the `function` definition, and variable types are declared before the assignment.


```r
cppFunction("
	int fun() {
		int x = 37;
		return x;
	}
")

fun()
```

```
## [1] 37
```



```r
cppFunction("
	int the_answer() {
		return 42;
	}				
")

storage.mode(the_answer())
```

```
## [1] "integer"
```

```r
storage.mode(42L)
```

```
## [1] "integer"
```

```r
identical(the_answer(), 42L)
```

```
## [1] TRUE
```



```r
cppFunction("
double euclidean_distance(double x, double y) {
	return sqrt(x * x + y * y);
}
")

euclidean_distance(1.5, 2.5)
```

```
## [1] 2.915476
```

```r
storage.mode(euclidean_distance(1.5, 2.5))
```

```
## [1] "double"
```

## Debugging `Rcpp`

Errors, mistakes, and broken code is a natural part of the process. Use the `Rprintf()` function to print from a `C++` function to our `R` instance.

It takes a string in double quotes and a place holder with a variable to replace it's value.


```r
cppFunction('
int fun() {
	int x = 42;
	
	Rprintf("message, x = %d\\n", x);
	
	return 76;
}
')

fun()
```

```
## message, x = 42
```

```
## [1] 76
```

Stop functions can be used in a `C++` function, thus `tryCatch` in R still works as status codes are unchanged.

Note that `%d` and `%s` are used, first for an `integer` and the second for a `string` as string interpolation is done differently in `C++`.


```r
# Define the function add()
cppFunction('
int add(int x, int y) {
	int res = x + y;
	Rprintf("** %d + %d = %d\\n", x, y, res);
	return res;
}
')

# Call add() to print THE answer
add(40, 2)
```

```
## ** 40 + 2 = 42
```

```
## [1] 42
```

Here we define a function that will only work if both arguments are positive, an error is thrown if either are negative, this is basically the same in `R`.


```r
cppFunction('
// adds x and y, but only if they are positive
int add_positive_numbers(int x, int y) {
	// if x is negative, stop
	if(x < 0) stop("x is negative");
	
	// if y is negative, stop
	if(y < 0) stop("y is negative");
	
	return x + y;
}
')

# Call the function with positive numbers
add_positive_numbers(2, 3)
```

```
## [1] 5
```

```r
# Call the function with a negative number
tryCatch({
	add_positive_numbers(-2, 3)
}, error = function(e) {
	message("Error: ", e)
})
```

```
## Error: Error in add_positive_numbers(-2, 3): x is negative
```

# Functions and control flow

## `C++` header files

Header files are like libraries which contain functions that can be used in any programme using the # directive. These are declared at the top of a `C++` programme and typically carry the `.h` extension. 

When writing `C++` for use in `R` Rcpp.h is often the only header file we will need but you can include others.

Name spaces are a way to group together objects so that they are separated from the global environment.

The statement following statement at the top of a `C++` programme destined for us in `R` attaches the functions or objects to the Rcpp `namespace` this can then be called in R with: `Rcpp::some_function()`:

`using namespace Rcpp;`

When writing code in `C++` you can have internal functions, this is default. To use functions in R you must export them; these are "exported functions" the following statement accomplishes this: 

`// [[Rccp::export]]`

Finally `sourceCpp()` is used to call our C++ code.

We have some `.cpp` files in the `cpp` directory, here is an example.


```bash
cat cpp/1-cpp.cpp
```

```
## // Include the Rcpp.h header
## #include <Rcpp.h>
## 
## // Use the Rcpp namespace
## using namespace Rcpp;
## 
## // [[Rcpp::export]]
## int the_answer() {
## 	// Return 42
## 	return 42;
## }
## 
## /*** R
## # Call the_answer() to check you get the right result
## the_answer()
## */
```


```r
Rcpp::sourceCpp("./cpp/1-cpp.cpp")
```

```
## 
## > the_answer()
## [1] 42
```

## For loops in `C++`

Note that you can the use your `C++` functions in the same chunk by declaring a mini `R` chunk inside the `Rcpp` chunk.


```cpp
# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double sqrt_approx(double value, int n) {
	double x = 1;
	for(int i = 0; n > i; i++) {
		x = (x + value / x) / 2.0;
	}
	
	return x;
}
```


```r
sqrt_approx(2, 10)
```

```
## [1] 1.414214
```

### Break for loop


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List sqrt_approx(double value, int n, double threshold) {
	double x = 1.0;
	double previous = x;
	bool is_good_enough = false;
	int i;
	for(i = 0; i < n; i++) {
		previous = x;
		x = (x + value / x) / 2.0;
		is_good_enough = fabs(previous - x) < threshold;
		
		// If the solution is good enough, then "break"
		Rprintf("** Index: %i, value: %f\n", i, x);
		if(is_good_enough) break;
	}
	return List::create(_["i"] = i , _["x"] = x);
}
```


```r
sqrt_approx(2, 1000, 0.1)
```

```
## ** Index: 0, value: 1.500000
## ** Index: 1, value: 1.416667
```

```
## $i
## [1] 1
## 
## $x
## [1] 1.416667
```

## While loops and do while


```cpp
# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double sqrt_approx(double value, double threshold) {
	double x = 1.0;
	double previous = x;
	bool is_good_enough = false;
	
	while(!is_good_enough) {
		previous = x;
		x = (x + value / x) / 2.0;
		is_good_enough = fabs(x - previous) < threshold;
	}
	
	return x;
}
```


```r
sqrt_approx(2, 0.00001)
```

```
## [1] 1.414214
```


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double sqrt_approx(double value, double threshold) {
	double x = 1.0;
	double previous = x;
	bool is_good_enough = false;
	
	// Initiate do while loop
	do {
		  previous = x;
		  x = (x + value / x) / 2.0;
		  is_good_enough = fabs(x - previous) < threshold;
	// Specify while condition
	} while(!is_good_enough);
	
	return x;
}
```


```r
sqrt_approx(2, 0.00001)
```

```
## [1] 1.414214
```

# Data structures

## `Rccp` classes and vectors

`Rcpp` facilitates things by providing a set of `C++` classes this helps in writing `C++` for `R`.

- `NumericVector`: to manipulate `numeric` vectors `c(1, 2, 3)`.
- `IntegerVector`: for integers `1:3`.
- `LogicalVector`: `c(TRUE, FALSE, TRUE)`.
- `CharacterVector`: strings `c("a", "b", "c")`.
- `List`: vectors of arbitrary R objects.

Methods are provided and can be used on the objects:

- `x.size()`
- `x[i]` gets element of index from vector.

Please note another big change in `C++` indexing starts at 0. To get the last value of a vector you can do `int n = x.size();` `x[n - 1];`.


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double first_plus_last(NumericVector x) {
	// The size of x
	int n = x.size();
	// The first element of x
	double first = x[0];
	// The last element of x
	double last = x[n-1];
	return first + last;
}
```


```r
x <- c(6, 28, 496, 8128)

first_plus_last(x)
```

```
## [1] 8134
```

```r
# Does the function give the same answer as R?
all.equal(first_plus_last(x), x[1] + x[4], 6 + 8128)
```

```
## [1] TRUE
```

Here we create a simple summing function in `C++` using the `NumericVector` class.


```cpp
# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double sum_cpp(NumericVector x) {
	int n = x.size();
	double result = 0;
	
	for(int i = 0; i < n; i++) {
		result = x[i] + result;
	}
	
	return result;
}
```


```r
set.seed(42)
x <- rnorm(1e6)

sum_cpp(x)
```

```
## [1] 573.7398
```

```r
# Does the function give the same answer as R's sum() function?
identical(sum_cpp(x), sum(x))
```

```
## [1] FALSE
```

### Vectors

`Rcpp` will try to convert or coerce a vector to a numeric vector when using the `NumericVector` class. If it cannot an exception will be raised.

If you want to create a vector of a given size inside C++ then you can do it in the following way, this is called vector initialisation and should be used in `R` code as well for speed and memory efficiency:


```cpp
# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]

NumericVector ones(int n) {
	// Initialise numeric vector of size n
	NumericVector x(n);
	
	for(int i = 0; i < n; i++) {
		x[i] = 1;
	}
	return x;
}
```


```r
ones(10)
```

```
##  [1] 1 1 1 1 1 1 1 1 1 1
```

Create a vector from a set of values and naming the vector elements, name the elements with an underscore and brackets with double quotes:


```cpp
# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector named_vector() {
	return NumericVector::create(_["a"] = 1, _["b"] = 2, 5, _["d"] = 10);
};
```


```r
named_vector()
```

```
##  a  b     d 
##  1  2  5 10
```

`C++` references vectors in order to copy them, use the clone function to create a deep copy.


```cpp
# include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector positives(NumericVector x) {
	NumericVector y = clone(x);
	
	return y;
}
```


```r
positives(1:10)
```

```
##  [1]  1  2  3  4  5  6  7  8  9 10
```


```cpp
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
```


```r
lo <- -2
hi <- 5
seq_cpp(lo, hi)
```

```
## [1] -2 -1  0  1  2  3  4  5
```

```r
# Does it give the same answer as R's seq() function?
identical(seq_cpp(lo, hi), seq(lo, hi))
```

```
## [1] TRUE
```

### List

You can create the `C++` equivalent of a list this allows you to return named lists with multiple element types.


```cpp
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
```


```r
create_vectors()
```

```
## $polygons
## [1] "triangle" "square"   "pentagon"
## 
## $mersenne_primes
##  first second  third 
##      3      7     31
```

## Vector cloning

As stated before cloning or copying objects in `C++` is an essential tool as object manipulation typically happens by reference in `C++`.

In these functions you can see that the first returns the original inputs twice, as the modified object is not returned only the reference to that object which was not itself modified. Making a deep copy allows manipulation and to return the result.

You can see the "error" here where we try to make a copy of the object by a simple assignment: `NumericVector the_copy = the_original;`


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List change_negatives_to_zero(NumericVector the_original) {
	// Set the copy to the original
	NumericVector the_copy = the_original;
	int n = the_original.size();
	for(int i = 0; i < n; i++) {
		if(the_copy[i] < 0) the_copy[i] = 0;
	}
	return List::create(_["the_original"] = the_original, _["the_copy"] = the_copy);
}

// [[Rcpp::export]]
List change_negatives_to_zero_with_cloning(NumericVector the_original) {
// Clone the original to make the copy
NumericVector the_copy = clone(the_original);
	int n = the_original.size();
	for(int i = 0; i < n; i++) {
		if(the_copy[i] < 0) the_copy[i] = 0;
	}
	return List::create(_["the_original"] = the_original, _["the_copy"] = the_copy);
}
```


```r
x <- c(0, -4, 1, -2, 2, 4, -3, -1, 3)
change_negatives_to_zero(x)
```

```
## $the_original
## [1] 0 0 1 0 2 4 0 0 3
## 
## $the_copy
## [1] 0 0 1 0 2 4 0 0 3
```

```r
# Need to define x again because it's changed now
x <- c(0, -4, 1, -2, 2, 4, -3, -1, 3)
change_negatives_to_zero_with_cloning(x)
```

```
## $the_original
## [1]  0 -4  1 -2  2  4 -3 -1  3
## 
## $the_copy
## [1] 0 0 1 0 2 4 0 0 3
```

## Weighted mean algorithms

`R` has a function weighted mean, it's the `mean / sum(weights)`. This `R` function calls `C++` implementations, but to be used in `R` it must use 3 different loops. We can do better with pure `C++`.


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double weighted_mean_cpp(NumericVector x, NumericVector w) {
	// Initialise these to zero
	double total_w = 0;
	double total_xw = 0;
	
	// Set n to the size of x
	int n = x.size();
	
	for(int i = 0; i < n; i++) {
		// Add the ith weight
		total_w += w[i];
		
		// Add the ith data value times the ith weight
		total_xw += x[i] * w[i];
	}
	
	return total_xw / total_w;
}
```


```r
x <- c(0, 1, 3, 6, 2, 7, 13, 20, 12, 21, 11)
w <- 1 / seq_along(x)
weighted_mean_cpp(x, w)
```

```
## [1] 4.423075
```

```r
# Does the function give the same results as R's weighted.mean() function?
identical(weighted_mean_cpp(x, w), weighted.mean(x, w))
```

```
## [1] FALSE
```


```r
res <- microbenchmark::microbenchmark(
	weighted_mean_cpp = weighted_mean_cpp(x, w),
	weighted.mean_R = weighted.mean(x, w),
	times = 3
)

ggplot2::autoplot(res)
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmark-weighted-mean-plot-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

Here we have the same basic function, however, if fed an `NA` value `NA` is returned.


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double weighted_mean_cpp(NumericVector x, NumericVector w) {
	double total_w = 0;
	double total_xw = 0;
	
	int n = x.size();
	
	for(int i = 0; i < n; i++) {
		// If the ith element of x or w is NA then return NA
		if(NumericVector::is_na(x[i]) || NumericVector::is_na(w[i])) {
			return NumericVector::get_na();
		}
		
		total_w += w[i];
		total_xw += x[i] * w[i];
	}
	
	return total_xw / total_w;
}
```


```r
x <- c(0, 1, 3, 6, 2, 7, 13, NA, 12, 21, 11)
w <- 1 / seq_along(x)
weighted_mean_cpp(x, w)
```

```
## [1] NA
```

## Vectors from STL

In `C++` it can become expensive to deal with vectors if not done properly. One must state the size of a vector at initialisation If we do not know the size to begin with we can make copies of the whole vector but this can become expensive. An alternative is to write a simple algorithm; determine the size first, then instantiate the vector at the correct size.

In this next section we show how bad `C++` can be slower if you don't handle your vectors correctly.


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector bad_select_positive_values_cpp(NumericVector x) {
	NumericVector positive_x(0);
	for(int i = 0; i < x.size(); i++) {
		if(x[i] > 0) {
			positive_x.push_back(x[i]);
		}
	}
	return positive_x;
}

// [[Rcpp::export]]
NumericVector good_select_positive_values_cpp(NumericVector x) {
	int n_elements = x.size();
	int n_positive_elements = 0;
	
	// Calculate the size of the output
	for(int i = 0; i < n_elements; i++) {
		// If the ith element of x is positive
		if(x[i] > 0) {
			// Add 1 to n_positive_elements
			n_positive_elements++;
		}
	}
	
	// Allocate a vector of size n_positive_elements
	NumericVector positive_x(n_positive_elements);
	
	// Fill the vector
	int j = 0;
	for(int i = 0; i < n_elements; i++) {
		// If the ith element of x is positive
		if(x[i] > 0) {
			// Set the jth element of positive_x to the ith element of x
			positive_x[j] = x[i];
			// Add 1 to j
			j++;
		}
	}
	return positive_x;
}
```


```r
set.seed(42)
x <- rnorm(1e4)
# Does it give the same answer as R?
all.equal(good_select_positive_values_cpp(x), x[x > 0])
```

```
## [1] TRUE
```

```r
# Which is faster?
```


```r
res <- microbenchmark::microbenchmark(
	bad_cpp = bad_select_positive_values_cpp(x),
	good_cpp = good_select_positive_values_cpp(x)
)

ggplot2::autoplot(res)
```

```
## Coordinate system already present. Adding new coordinate system, which will replace the existing one.
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmark-good-vs-bad-vectors-cpp-plot-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

We can get a vector class from the `std` library, these have methods attached which allow efficient manipulation of vectors without having to write all that code we did previously. As you can see our code is much simpler. Just declare your return of a `std::vector<double>`.

The library `std` from `stl` is a library for `C++` that has a lot of algorithms for easily manipulating data structures such as vectors. You could use the `vector` class from `stl` but you would not have access to `Rcpp`'s methods such as: `mean()`, `round()`, `abs()`.



```cpp
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
```


```r
set.seed(42)
x <- rnorm(1e6)
# Does it give the same answer as R?
all.equal(select_positive_values_std(x), x[x > 0])
```

```
## [1] TRUE
```

```r
# Which is faster?
```


```r
res <- microbenchmark::microbenchmark(
	good_cpp = good_select_positive_values_cpp(x),
	std = select_positive_values_std(x),
	times = 3
)

ggplot2::autoplot(res)
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmark-std-easier-vectors-plot-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

# Random number generation

## Scalar random number generation

Note here we're using `R`'s `rnorm()` function inside `C++`.


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector positive_rnorm(int n, double mean, double sd) {
	// Specify out as a numeric vector of size n
	NumericVector out(n);
	
	// This loops over the elements of out
	for(int i = 0; i < n; i++) {
		// This loop keeps trying to generate a value
		do {
			// Call R's rnorm()
			out[i] = R::rnorm(mean, sd);
			// While the number is negative, keep trying
			
			Rprintf("Searching for positive: %d\n", i);
		} while(out[i] <= 0);
	}
	return out;
}
```


```r
positive_rnorm(10, 2, 2)
```

```
## Searching for positive: 0
## Searching for positive: 1
## Searching for positive: 2
## Searching for positive: 3
## Searching for positive: 4
## Searching for positive: 5
## Searching for positive: 6
## Searching for positive: 7
## Searching for positive: 8
## Searching for positive: 8
## Searching for positive: 9
```

```
##  [1] 1.5355383 2.8431059 1.2788587 2.1408923 3.5997502 3.7482570 1.2109167
##  [8] 4.8478022 2.1352723 0.3937712
```

## Sampling from a mixture of distributions


```cpp
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
```


```r
weights <- c(0.3, 0.7)
# Randomly choose a component 5 times
replicate(5, choose_component(weights, sum(weights)))
```

```
## [1] 1 1 1 1 1
```

The complete algorithm for sampling a mixture distribution:

1. Choose component.
1. Generate a normal random number using the mean and standard deviation of the selected component.


```cpp
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
```


```r
weights <- c(0.3, 0.7)
means <- c(2, 4)
sds <- c(2, 4)
rmix(10, weights, means, sds)
```

```
##  [1]  3.773930  1.304893  1.478971  2.848569 -2.568089  4.303119  1.200066
##  [8]  4.010071  3.265160  3.682132
```

## Rolling operations

### Moving average, rolling mean


```r
rollmean1 <- function(x, window = 3) {
	n <- length(x)
	res <- rep(NA, n)
	for (i in seq(window, n)) {
		res[i] <- mean(x[seq(i - window + 1, i)])
	}
	return(res)
}
```


```r
rollmean2 <- function(x, window = 3){
	n <- length(x)
	res <- rep(NA, n)
	total <- sum(head(x, window))
	res[window] <- total / window
	for(i in seq(window + 1, n)) {
		total <- total + x[i] - x[i - window]
		res[i] <- total / window
	}
	return(res)
}
```


```r
rollmean3 <- function(x, window = 3) {
	# Add the first window elements of x
	initial_total <- sum(head(x, window))
	
	# The elements to add at each iteration    
	lasts <- tail(x, - window)
	
	# The elements to remove
	firsts <- head(x, - window)
	
	# Take the initial total and add the 
	# cumulative sum of lasts minus firsts
	other_totals <- initial_total + cumsum(lasts - firsts)
	
	# Build the output vector 
	c(
		rep(NA, window - 1), # leading NA
		initial_total / window, # initial mean
		other_totals / window   # other means
	)
}
```


```r
x <- rnorm(1e4)
# This checks rollmean1() and rollmean2() give the same result
all.equal(rollmean1(x), rollmean2(x))
```

```
## [1] TRUE
```

```r
# This checks rollmean1() and rollmean3() give the same result
all.equal(rollmean1(x), rollmean3(x))
```

```
## [1] TRUE
```


```r
res <- microbenchmark::microbenchmark(
	rollmean1(x), 
	rollmean2(x), 
	rollmean3(x),
	times = 5
)

ggplot2::autoplot(res)
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmark-rollmeans-plots-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

Now let's compare performance calculating rolling means with `C++`. 

Note that `rollmean3()` uses vectorised operations, thus it shows good performance. However, difficult to read for a debutant in `R`. The second version can be converted to `C++` more easily as it contains less of `R`'s idiosynchronies - bad `R` code can be good `C++`.


```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector rollmean_cpp(NumericVector x, int window) {
	int n = x.size();
	
	// Set res as a NumericVector of NAs with length n
	NumericVector res(n, NumericVector::get_na());
	
	// Sum the first window worth of values of x
	double total = 0.0;
	for(int i = 0; i < window; i++) {
		total += x[i];
	}
	
	// Treat the first case seperately
	res[window - 1] = total / window;
	
	// Iteratively update the total and recalculate the mean 
	for(int i = window; i < n; i++) {
		// Remove the (i - window)th case, and add the ith case
		total += - x[i - window] + x[i];
		// Calculate the mean at the ith position
		res[i] = total / window;
	}
	
	return res;  
}
```


```r
# Compare rollmean2, rollmean3 and rollmean4   
set.seed(42)
x <- rnorm(1e4)
```



```r
res <- microbenchmark::microbenchmark(
	rollmean1(x, 4),
	rollmean2(x, 4), 
	rollmean3(x, 4),
	rollmean_cpp(x, 4),
	times = 5
)

ggplot2::autoplot(res)
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmark-cpp-rolling-mean-plot-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

### Last observation forward

If missing values carry the last one not missing. In timeseries it is common to carry the last value that wasn't missing forward. This is known as last observation carried forward.


```r
na_locf_R <- function(x) {
	current <- NA  
	res <- x
	for(i in seq_along(x)) {
		if(is.na(x[i])) {
			# Replace with current
			res[i] <- current
		} else {
			# Set current 
			current <- x[i]
		}
	}
	
	return(res)
}
```



```cpp
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector na_locf_cpp(NumericVector x) {
	// Initialise to NA
	double current = NumericVector::get_na();
	
	int n = x.size();
	NumericVector res = clone(x);
	for(int i = 0; i < n; i++) {
		// If ith value of x is NA
		if(NumericVector::is_na(x[i])) {
			// Set ith result as current
			res[i] = current;
		} else {
			// Set current as ith value of x
			current = res[i];
		}
	}
	return res;
}
```


```r
set.seed(42)
x <- rnorm(1e5)
# Sprinkle some NA into x
x[sample(1e5, 100)] <- NA
```



```r
res <- microbenchmark::microbenchmark(
	na_locf_R(x),
	na_locf_cpp(x),
	times = 5
)

ggplot2::autoplot(res)
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmark-last-obs-forward-cpp-plot-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

### Mean carried forward

An alternative to just carrying the last value forward is to calculate the mean of the last observations and carry that forward.


```r
na_meancf_R <- function(x) {
	total_not_na <- 0
	n_not_na <- 0
	res <- x
	for (i in seq_along(x)) {
		if (is.na(x[i])) {
			res[i] <- total_not_na / n_not_na
		} else {
			total_not_na <- total_not_na + x[i]
			n_not_na <- n_not_na + 1
		}
	}
	return(res)
}

na_meancf_R(c(1, 2, 3, 212, 3, NA, 14, 13, NA, 5, 15, 41, 34, 3, 3))
```

```
##  [1]   1.00000   2.00000   3.00000 212.00000   3.00000  44.20000  14.00000
##  [8]  13.00000  35.42857   5.00000  15.00000  41.00000  34.00000   3.00000
## [15]   3.00000
```


```cpp
#include <Rcpp.h>
using namespace Rcpp; 

// [[Rcpp::export]]
NumericVector na_meancf_cpp(NumericVector x) {
	double total_not_na = 0.0;
	double n_not_na = 0.0;
	NumericVector res = clone(x);
	
	int n = x.size();
	for(int i = 0; i < n; i++) {
		// If ith value of x is NA
		if(NumericVector::is_na(x[i])) {
			// Set the ith result to the total of non-missing values 
			// divided by the number of non-missing values
			res[i] = total_not_na / n_not_na;
		} else {
			// Add the ith value of x to the total of non-missing values
			total_not_na += x[i];
			// Add 1 to the number of missing values
			n_not_na++;
		}
	}
	return res;
}
```


```r
set.seed(42)
x <- rnorm(1e5)
x[sample(1e5, 100)] <- NA
```


```r
res <- microbenchmark::microbenchmark( 
	na_meancf_R(x), 
	na_meancf_cpp(x), 
	times = 5
)

ggplot2::autoplot(res)
```

<img src="/figures/cs-Rcpp_files/figure-html/benchmark-na-meancf-cpp-plot-1.png" style="display: block; margin: auto;" />

```r
remove(res)
```

## Auto-regressive (AR) model

Auto-regressive models are a type of linear regression for time series. The predicted values depend on the values from the previous time points. Implimenting an auto-regressive algorithm is naturally an iterative process. This is best implmented in `C++`. Here is a version in `R`, this can be translated to `C++`.


```r
ar_R <- function(n, constant, phi, eps) {
	p <- length(phi)
	x <- numeric(n)
	for(i in seq(p + 1, n)) {
		value <- rnorm(1, constant, eps)
		for(j in seq_len(p)) {
			value <- value + phi[j] * x[i - j]
		}
		x[i] <- value
	}
	return(x)
}
```

Algorithms that use iteration are best done in C++.


```cpp
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
```


```r
d <- data.frame(
	x = 1:50,
	y = ar_cpp(50, 10, c(1, -0.5), 1)
)

ggplot(d, aes(x, y)) +
	geom_line() +
	labs(title = "Auto regressive model")
```

<img src="/figures/cs-Rcpp_files/figure-html/auto-regressive-cpp-1.png" style="display: block; margin: auto;" />

```r
remove(d)
```

## Moving average (MA) model

Moving averages also depend on previous calculations. 


```r
ma_R <- function(n, mu, theta, sd) {
	q <- length(theta)
	x <- numeric(n)
	eps <- rnorm(n, 0, sd)
	for(i in seq(q + 1, n)) {
		value <- mu + eps[i]
		for(j in seq_len(q)) {
			value <- value + theta[j] * eps[i - j]
		}
		x[i] <- value
	}
	return(x)
}
```



```cpp
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
```


```r
d <- data.frame(
	x = 1:50,
	y = ma_cpp(50, 10, c(1, -0.5), 1)
)

ggplot(d, aes(x, y)) +
	geom_line() +
	labs(title = "Moving average")
```

<img src="/figures/cs-Rcpp_files/figure-html/moving-average-cpp-1.png" style="display: block; margin: auto;" />

```r
remove(d)
```

## Auto regressive moving average (ARMA) model


```cpp
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
```


```r
d <- data.frame(
	x = 1:50,
	y = arma(50, 10, c(1, -0.5), c(1, -0.5), 1)
)

ggplot(d, aes(x, y)) +
	geom_line() +
	labs(title = "ARMA model")
```

<img src="/figures/cs-Rcpp_files/figure-html/arma-model-1.png" style="display: block; margin: auto;" />

```r
remove(d)
```
