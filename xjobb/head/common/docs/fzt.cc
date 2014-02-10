/*
 * This is the 'trivial' Fast Zeta Transform, in exponential time
 * and space. The vector f contains sets in binary code, with
 * each element in f corresponding to the function value of the
 * set whose bitset is the index of the element.
 * Example: the set {1,3,4} is encoded as 11010 (for 5 elements), 
 * and its function value is f(11010) = f(26).
 *
 * The function iterates over all subsets of each set and sums
 * the function values, overwriting the existing vector elements 
 * with the sums.
 *
 * This function works as a utility function for the Fast Zeta 
 * Transform in Linear Space algorithm.
 */
void utils::fast_down_zeta_transform_exp_space(
	const u_int_t& n, 
	rval_list_t& f) 
{
	for (u_int_t j = U_ONE; j <= n; ++j) {
		u_int_t index = U_ZERO;
		u_int_t step = exp2(j - 1);	// 2^(j-1)
		while (index != f.size) {
			index += step;
			for (u_int_t i = U_ZERO; i < step; ++i) {
				add_assign(f[index], f[index - step]);
				++index;
			}
		}
	}
} 
