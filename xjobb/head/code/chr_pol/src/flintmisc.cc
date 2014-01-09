#include <flint/fmpz_poly.h>
#include <flint/fmpz.h>
#include <sstream>
#include <string.h>
#include "types.h"
#include "interface.h"

// DEBUG
#include <iostream>

/*
 * Various functions for interfacing FLINT with the program.
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
 *
 * garbage collection:
 * save_sp (save stack pointer)
 * gb (restore stack pointer)
 */

/*
 * Initializes a zero
 */
void interface::init_zero(rval_t& pol) {
	fmpz_poly_init(pol);

	fmpz_poly_zero(pol);
	// print polynomial
	//fmpz_poly_print(pol);
}

/*
 * Initializes a polynomial (ring value type) to a monomial of degree deg.
 */
void interface::init_monomial(const u_int_t& deg, rval_t& pol) {
	fmpz_poly_init2(pol, deg);

	fmpz_poly_set_coeff_si(pol, deg, 1);

	/*
	std::cout << "A monomial: ";
	fmpz_poly_print_pretty(pol, "x");
	std::cout << std::endl;
	*/
}

/*
 * pol1 += pol2
 */
void interface::add_assign(rval_t& pol1, rval_t& pol2) {

	/*
	fmpz_poly_print_pretty(pol1, "x");
	std::cout << " + ";
	fmpz_poly_print_pretty(pol2, "x");
	std::cout << " = ";
	*/

	fmpz_poly_add(pol1, pol1, pol2);

	/*
	fmpz_poly_print_pretty(pol1, "x");
	std::cout << std::endl;
	*/
}

void interface::mul_assign(rval_t& pol1, rval_t& pol2, const u_int_t& n) {

	/*
	fmpz_poly_print_pretty(pol1, "x");
	std::cout << " * ";
	fmpz_poly_print_pretty(pol2, "x");
	std::cout << " = ";
	*/
	
	fmpz_poly_mul(pol1, pol1, pol2);

	/*
	fmpz_poly_print_pretty(pol1, "x");
	std::cout << std::endl;
	*/

	// Truncate away all high-degree terms.
	fmpz_poly_truncate(pol1, n);

	/*
	std::cout << "After trucating: ";
	fmpz_poly_print_pretty(pol1, "x");
	std::cout << std::endl;
	*/
}

void interface::power(rval_t& pol, const u_int_t& deg, const u_int_t& n) {
	/*	
	fmpz_poly_print_pretty(pol, "x");
	std::cout << " ^" << deg;
	std::cout << " = ";
	*/

	fmpz_poly_pow_trunc(pol, pol, deg, n + 1);
	
	/*
	fmpz_poly_print_pretty(pol, "x");
	std::cout << std::endl;
	*/
}

std::string* interface::print_coeff(rval_t& pol, const u_int_t&) {
	/*
	fmpz* lead_coeff = fmpz_poly_lead(pol);
	if (lead_coeff == NULL) {
		std::cout << "IT WAS NULL\n";
		return &ZERO_STR;
	}
	fmpz_print(lead_coeff);

	std::string* s = new std::string();
	return s;
*/

	fmpz* lead_coeff = fmpz_poly_lead(pol);
	if (lead_coeff == NULL) {
		return &ZERO_STR;
	}
	char* c = NULL;
	c = fmpz_get_str(c, 10, lead_coeff);
	std::string* s = new std::string(c);
	free(c);
	return s;
	
}

void interface::flip_sign(rval_t& pol, int sign) {
	if (sign > 0)
		return;
	fmpz_poly_neg(pol, pol);	// sets pol to -pol
}
	
void interface::print_interpolate(std::string** _points, const u_int_t& n) {
	std::cout << "Preparing to interpolate ... ";
	fmpz* x = _fmpz_vec_init(n + 1);
	fmpz* y = _fmpz_vec_init(n + 1);
	for (u_int_t i = U_ZERO; i - 1 != n; ++i) {
		std::cout << "the point is: " << *(_points[i]) << std::endl;

		fmpz_set_ui(x + i, i);
		char* c = new char[_points[i]->length() + 1];
		strcpy(c, _points[i]->c_str());
		fmpz_set_str(y + i, c, 10);
		delete[] c;
	}

	fmpz_poly_t chr_pol;
       	fmpz_poly_init2(chr_pol, n);

	std::cout << "interpolating ... ";

	fmpz_poly_interpolate_fmpz_vec(chr_pol, x, y, n + 1);

	std::cout << "Done!\n";

	_fmpz_vec_clear(x, n);
	_fmpz_vec_clear(y, n);

	std::cout << "Chromatic polynomial: " << std::endl;
	
	fmpz_poly_print_pretty(chr_pol, "x");

	std::cout << std::endl;
}
