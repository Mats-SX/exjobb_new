#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <fstream>
#include <string>

namespace utils {

	/* Optimizations */
	bool trivial(const u_int_t&, const u_int_t&, bool**);
	void parallel(rval_t*, const set_t&, const set_t&, const u_int_t&, const u_int_t&, 
			const set_t&, const u_int_t&, const u_int_t&, 
			const u_int_t&, bool**, pth_t*);

	/* FZT */
	void fast_down_zeta_transform_exp_space(const u_int_t&, rval_list_t&);
	void fast_up_zeta_transform_exp_space(const u_int_t&, rval_list_t&);

	std::string* count_colourings_small_space(const u_int_t&, const u_int_t&, 
			bool**, u_int_t&);

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


#endif
