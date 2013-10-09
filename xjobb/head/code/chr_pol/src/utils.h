#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <fstream>

namespace utils {

	/* FZT */
	void fast_down_zeta_transform_exp_space(const u_int_t&, rval_list_t&);
	void fast_up_zeta_transform_exp_space(const u_int_t&, rval_list_t&);

	void small_space_chr_pol(const u_int_t&, const u_int_t&, bool**);
	
	/* Sets */
	u_int_t size_of(const set_t&);
	bool is_empty(const set_t&);
	void neighbours_of(const set_t&, bool**, const u_int_t&, set_t&);
	bool independent(const set_t&, bool**, const u_int_t&);
	set_list_t* members_in(const set_t&);
	
	/* Misc. */
	sign_t exp_neg_one(const u_int_t&);
	u_int_t exp2(const u_int_t&);
	void part_n(const u_int_t&, u_int_t&, u_int_t&);
	void parse(std::ifstream&, bool**, u_int_t&);
}

/*
 * These methods are implemented independently for each polynomial library used.
 * Their implementations are found in <lib>misc.cc, where <lib> is the name of the 
 * library (such as NTL, PARI, etc).
 */
namespace interface {
	void init_zero(rval_t&);
	void init_monomial(const u_int_t&, rval_t&);
	void add_assign(rval_t&, rval_t&);
	void mul_assign(rval_t&, rval_t&);
	void power(rval_t&, const u_int_t&);
	void print_coeff(rval_t&, const u_int_t&);
	void flip_sign(rval_t&, int);
}

#endif
