#include <NTL/ZZX.h>
#include <NTL/ZZ_p.h>
#include <NTL/vec_ZZ_p.h>
#include <sstream>

#include "types.h"
#include "interface.h"

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
 * pol is a ZZX 
 */
void interface::init_zero(rval_t&) {
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

void interface::mul_assign(rval_t& pol1, rval_t& pol2, const u_int_t&) {
	pol1 *= pol2;
	
	/* This would keep polynomials at minimum degree.
	 * It does not work, which is strange.
	 */
//	if (NTL::deg(pol1) > n)
//		pol1.SetLength(n+1);
}

void interface::power(rval_t& pol, const u_int_t& deg, const u_int_t& n) {
	rval_t cpy(pol);
	for (u_int_t i = U_ZERO; i < deg - U_ONE; ++i) {
		pol *= cpy;
	}
	if (NTL::deg(pol) > n)	// OK with signed/unsigned comparison
		pol.SetLength(n+1);
}

std::string* interface::print_coeff(rval_t& pol, const u_int_t&) {
	//std::cout << pol[n] << std::endl;
	std::ostringstream os;
//	std::cout << "pol:" << pol << ", n:" << n << ", deg:" << NTL::deg(pol) << std::endl;
	os << NTL::LeadCoeff(pol);
	std::string* s = new std::string(os.str());
	//std::cout << "Evaluated x(t) to " << *s << std::endl;
	return s;
}

void interface::flip_sign(rval_t& pol, int sign) {
	if (sign > 0)
		return;
	pol *= S_N_ONE;
}

/*
 * List is n + 1 elements large.
 * I use largest function value as modulus. Does it hold, theoretically? TODO
 */
void interface::print_interpolate(std::string** _points, const u_int_t& n) {
	std::cout << "Preparing to interpolate ... ";
	NTL::ZZ_pX chr_pol;
	NTL::ZZ prime;
	std::istringstream iss(*_points[n]);
	iss >> prime;

	NTL::NextPrime(prime, prime);

	NTL::ZZ_p::init(prime);

	NTL::vec_ZZ_p x;	// 0, 1, 2, ...
	x.SetLength(n + 1);
	NTL::vec_ZZ_p y;	// fill from _points
	y.SetLength(n + 1);

	for (u_int_t i = U_ZERO; i - 1 != n; ++i) {
		x[i] = i;
		iss.str(*_points[i]);
		iss.clear();
		iss >> y[i];
	}

	std::cout << "Done!\nInterpolating... ";

	// This has every other coefficient mod prime, when it is supposed to be negative
	// No interpolation provided for non-mod integers...
	interpolate(chr_pol, x, y);

	std::cout << "Done!\n";

	std::cout << "Chromatic Polynomial: " << std::endl;
	// Put all coeffs back into ZZ types to allow negative coeffs.
	NTL::vec_ZZ coeffs;
	coeffs.SetLength(n + 1);
	std::stringstream oss;
	// only n steps; exploiting the fact that coeff of x^0 is 0 for any graph with n>0
	for (u_int_t i = U_ZERO; i < n; ++i) {
		if (i != U_ZERO) {
			std::cout << " + ";
		}
		u_int_t index = n - i;
		oss << chr_pol[index];
		oss >> coeffs[index];
		oss.clear();
		if (i % 2) {	// odd steps from leading coeff
			coeffs[index] -= prime; 
		}
		std::cout << coeffs[index] << "*x^" << index;
	}
	std::cout << std::endl;
}
