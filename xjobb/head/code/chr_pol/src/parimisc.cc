#include <pari/pari.h>
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
 *
 * garbage collection:
 * save_sp (save stack pointer)
 * gb (restore stack pointer)
 */


void interface::save_sp(sp& lbot) {
	lbot = avma;
}

void interface::gb(rval_t& r, sp& lbot) {
	//pari_printf("r before: %Ps\n", r);
	r = gerepile(interface::stp, lbot, r);
	//pari_printf("r after: %Ps\n", r);
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
	//pari_printf("pol1 before: %Ps\n pol2: %Ps\n", pol1, pol2);

	pol1 = ZX_add(pol1, pol2);

	//pari_printf("  pol1 after: %Ps\n", pol1);
	
}

void interface::mul_assign(rval_t& pol1, rval_t& pol2, const u_int_t& n) {

	pol1 = ZX_mul(pol1, pol2);

	// Truncate away all high-degree terms.
	if (lg(pol1) > n + 3)	
		setlg(pol1, n + 3);

}

void interface::power(rval_t& pol, const u_int_t& deg, const u_int_t& n) {
//	std::cout << "Powering ";
//	pari_printf("pol before power: %Ps\n", pol);
//	std::cout << " to " << deg << std::endl;

	pol = gpowgs(pol, deg);
	// try gen_powu(pol, deg, NULL, sqr, mul) ?

//	pari_printf("pol after power: %Ps\n", pol);
//
	// Truncate away all high-degree terms.
	if (degpol(pol) > n) {
		setlg(pol, n + 3);
	}
	
//	std::cout << "Result: ";
//	pari_printf("%Ps\n", pol);
	
}

std::string* interface::print_coeff(rval_t& pol, const u_int_t& n) {
	//pari_printf("%Ps\n", pol);
	std::string* s = new std::string(pari_sprintf("%Ps", leading_term(pol)));
	return s;
}

void interface::flip_sign(rval_t& pol, int sign) {
	if (sign > 0)
		return;
	pol = gmulgs(pol, sign);
}
	
void interface::print_interpolate(std::string** _points, const u_int_t& n) {
	std::cout << "Preparing to interpolate ... ";
	GEN x = zerovec(n + 1);
	GEN y = zerovec(n + 1);
	for (u_int_t i = U_ZERO; i - 1 != n; ++i) {
		gel(x, i+1) = stoi(i);
		gel(y, i+1) = gp_read_str(_points[i]->c_str());
	}

	GEN p = compo(y, n+1);
	do {
		p = addsi(1, p);
	} while (!isprime(p));
	
	std::cout << "interpolating ... ";
	GEN chr_pol = FpV_polint(x, y, p, 0);
	std::cout << "Done!\n";

	std::cout << "Chromatic polynomial: " << std::endl;
	for (u_int_t i = U_ZERO; i < n; ++i) {
		if (i != U_ZERO) {
			std::cout << " + ";
		}
		u_int_t index = n + 2 - i;
		GEN coeff = gel(chr_pol, index);
		if (i % 2)
			coeff = subii(coeff, p);
		pari_printf("%Ps", coeff);
		std::cout << "*x^" << index - 2;
	}
	std::cout << std::endl;
}
