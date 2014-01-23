/*
 * Executes the BHKK algorithm for a range of subsets starting
 * from start and ending with end, proceeding in in-order.
 */
void utils::parallel(
		rval_t* r,
		const set_t& start,
		const set_t& end,
		const u_int_t& two_to_the_n1,
		const u_int_t& two_to_the_n2,
		const set_t& v2,
		const u_int_t& n,
		const u_int_t& n2,
		const u_int_t& q,
		bool** matrix) {

	// For each subset X1 of V1
	for (set_t x1 = start; x1 <= end; ++x1) {

		// Arrays of polynomials
		rval_list_t l(two_to_the_n2);
		rval_list_t h(two_to_the_n2);

		// For each subset Z2 of V2, set h(Z2) <- 0
		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			init_zero(h[i]);
		}

		// For each independent subset Y1 of X1,
		// set h(V2 \ N(Y1)) <- h(V2 \ N(Y1)) + z^(|Y1|)
		for (set_t y1 = EMPTY_SET; y1 <= x1; ++y1) {
		  
			if ((y1 | x1) <= x1) { // <=> Y1 is subset of X1
				if (independent(y1, matrix, n)) {
					rval_t p;
					init_monomial(size_of(y1), p);
					set_t neighbours = EMPTY_SET;
					neighbours_of(y1, matrix, n, neighbours);

					add_assign(
					  h[(v2 & (~ neighbours)) / two_to_the_n1], p);
				}
			}
		}

		// For each independent subset Y2 of V2,
		// set l(Y2) <- z^(|Y2|)
		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			set_t y2 = i * two_to_the_n1;

			if (independent(y2, matrix, n)) {
				init_monomial(size_of(y2), l[i]);
			} else {
				init_zero(l[i]);
			}
		}

		// Set h <- hS'
		fast_up_zeta_transform_exp_space(n2, h);

		// Set h <- h*l
		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			mul_assign(h[i], l[i], n);
		}
		
		// Set h <- hS
		fast_down_zeta_transform_exp_space(n2, h);

		// For each subset X2 of V2,
		// set r <- r + (-1)^(n-|X1|-|X2|) * h(X2)^q
		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			set_t x2 = i * two_to_the_n1;
			int exponent = n - size_of(x1) - size_of(x2);
			int sign = exp_neg_one(exponent);

			power(h[i], q, n);
			flip_sign(h[i], sign);

			add_assign(*r, h[i]);
		}
	}
}