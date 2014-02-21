/*
 * This function calculates the Fast Zeta Transform in Linear Space for a given
 * problem type.
 *
 * n is split into n1 and n2, where n = n1 + n2 is assumed to hold.
 * family is a pointer to a list of the sets to be used for covering. Each 
 * member of the list is a set represented as a bit vector. A 1-bit in 
 * position i means that i is a member of the set.
 * f represents a function with a one-to-one mapping from each member of 
 * family to a value in an algebraic ring.
 */
void utils::fast_zeta_transform_linear_space(
		int_t n1,
		int_t n2,
		int_list_t* family,
		rval_list_t* f,
		int_t q,
		rval_t* pk)
{

	int_t two_to_the_n1 = pow(2, n1);		// 2^n1
	int_t two_to_the_n2 = pow(2, n2);		// 2^n2

	int_t u1 = two_to_the_n1 - 1;			// Index of U1
	int_t u2 = pow(2, n1+n2) - two_to_the_n1;	// Index of U2

	// Array g
	rval_list_t g(two_to_the_n2);

	// For each subset X1 of U1
	for (int_t x1 = 0; x1 < two_to_the_n1; ++x1) {

		// For each Y2 in U2, set g(Y2) <- 0
		for (int_t i = 0; i < two_to_the_n2; ++i) {
			g[i].set_degree(n1 + n2);
		}

		// For each Y in F, if YnU1 is a subset of X1, 
		// then set g(YnU2) <- g(YnU2) + f(Y)
		for (int_t i = 0; i < family->size; ++i) {
			int_t y = (*family)[i];

			// if YnU1 is a subset of X1
			if (((y & u1) | x1) <= x1) {
				g[(y & u2) / two_to_the_n1] += (*f)[i];
			}
		}

		// Compute h <- gS
		fast_zeta_transform_exp_space(n2, &g);

		// For each subset X2 of U2,
		// calculate sum according to problem
		for (int_t i = 0; i < two_to_the_n2; ++i) {
			int_t x2 = i * two_to_the_n1;
			int_t x = x1 | x2;
			
			// Sum differently dependent on problem
			// Below is the code for q-packing

			int_t size_of_X = count_1bits(x);

			g[i].raise_to_the(q);
			Polynomial p;
			p.set_degree(n1 + n2);
			mpz_set_ui(p[0], 1);
			mpz_set_ui(p[1], 1);		// p = 1 + z
			p.raise_to_the(size_of_X);
			p *= power_of_minus_one(n1 + n2 - size_of_X);
			g[i] *= p;
			
			(*pk) += g[i];
		}
	}
} 
