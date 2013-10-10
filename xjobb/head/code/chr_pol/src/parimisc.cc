#include <pari/pari.h>
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
 *
 * garbage collection:
 * save_sp (save stack pointer)
 * gb (restore stack pointer)
 */

sp interface::stp;

void interface::save_sp() {
	interface::stp = avma;
}

void interface::gb(rval_t& r) {
	sp lbot = avma;
	r = gerepile(interface::stp, lbot, r);
}

/*
 * pol is a GEN
 */
void interface::init_zero(rval_t& pol) {
	//std::cout << "Initializing zero-pol... " << std::endl;
	
	pol = pol_0(0);
}

/*
 * Initializes a polynomial (ring value type) to a monomial of degree deg.
 */
void interface::init_monomial(const u_int_t& deg, rval_t& pol) {
	
	//std::cout << "Initializing monomial of deg " << deg << std::endl;
	
	if (deg == U_ZERO) {
		pol = pol_1(0);
		return;
	}
	
	pol = cgetg(deg + 3, t_POL);
	setsigne(pol, S_ONE);
	setvarn(pol, U_ZERO);
	for (u_int_t i = 2; i < deg + 2; ++i) {
		gel(pol, i) = gen_0;
	}
	gel(pol, deg + 2) = gen_1;

	//std::cout << "Done!" << std::endl;
}

/*
 * pol1 += pol2
 */
void interface::add_assign(rval_t& pol1, rval_t& pol2) {

	pol1 = ZX_add(pol1, pol2);

	/*
	std::cout << "Result is: ";
	pari_printf("%Ps\n", pol1);
	*/
}

void interface::mul_assign(rval_t& pol1, rval_t& pol2, const u_int_t& n) {

	pol1 = ZX_mul(pol1, pol2);

	// Truncate away all high-degree terms.
	if (lg(pol1) > n + 3)	
		setlg(pol1, n + 3);

}

void interface::power(rval_t& pol, const u_int_t& deg, const u_int_t& n) {

	
//	std::cout << "Powering ";
//	pari_printf("%Ps", pol);
//	std::cout << " to " << deg << std::endl;

	pol = gpowgs(pol, deg);	
	// try gen_powu(pol, deg, NULL, sqr, mul) ?

	// Truncate away all high-degree terms.
	if (lg(pol) > n + 3)	
		setlg(pol, n + 3);
	
//	std::cout << "Result: ";
//	pari_printf("%Ps\n", pol);
	
}

void interface::print_coeff(rval_t& pol, const u_int_t& n) {
	pari_printf("%Ps\n", pol);
}

void interface::flip_sign(rval_t& pol, int sign) {
	if (sign > 0)
		return;
	pol = gmulgs(pol, sign);
}
