#include <NTL/ZZX.h>
#include "types.h"
#include "utils.h"

// DEBUG
#include <iostream>

/*
 * Various functions for interfacing PARI with the program.
 */

/*
 * The interface methods needed:
 * zero-polynomial initialization (init_zero)
 * monomial of arbitrary degree initialization (init_monomial)
 * addition (add_assign)
 * multiplication (mul_assign)
 * powering (power)
 * output of highest-def coefficient (print_last_coeff)
 * sign-flip (flip-sign)
 */

/*
 * pol is a GEN
 */
void interface::init_zero(rval_t& pol) {
	//std::cout << "Initializing zero-pol... " << std::endl;
}

/*
 * Initializes a polynomial (ring value type) to a monomial of degree deg.
 */
void interface::init_monomial(const u_int_t& deg, rval_t& pol) {
	NTL::SetCoeff(pol, deg);	
}

/*
 * pol1 += pol2
 */
void interface::add_assign(rval_t& pol1, rval_t& pol2) {
	pol1 += pol2;
}

void interface::mul_assign(rval_t& pol1, rval_t& pol2) {
	pol1 *= pol2;
}

void interface::power(rval_t& pol, const u_int_t& deg) {
	rval_t cpy(pol);
	for (u_int_t i = U_ZERO; i < deg - U_ONE; ++i) {
		pol *= cpy;
	}
}

void interface::print_coeff(rval_t& pol, const u_int_t& n) {
	std::cout << pol[n] << std::endl;
}

void interface::flip_sign(rval_t& pol, int sign) {
	if (sign > 0)
		return;
	pol *= S_N_ONE;
}
