#ifndef INTERFACE_H
#define INTERFACE_H

/*
 * These methods are implemented independently for each polynomial library used.
 * Their implementations are found in <lib>misc.cc, where <lib> is the name of the 
 * library (such as NTL, PARI, etc).
 */
namespace interface {
	void init_zero(rval_t&);
	void init_monomial(const u_int_t&, rval_t&);
	void add_assign(rval_t&, rval_t&);
	void mul_assign(rval_t&, rval_t&, const u_int_t&);
	void power(rval_t&, const u_int_t&, const u_int_t&);
	std::string* print_coeff(rval_t&, const u_int_t&);
	void print_interpolate(std::string**, const u_int_t&);
	void flip_sign(rval_t&, int);
}

#endif
