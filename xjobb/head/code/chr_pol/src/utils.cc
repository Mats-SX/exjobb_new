#include <iostream>
#include <unistd.h>

#include "utils.h"
#include "interface.h"
#include "types.h"

using namespace interface;

/* FZT */

/*
 * This is the 'trivial' Fast Zeta Transform, in exponential time and space.
 * The vector f contains sets in binary code, with each element in f corresponding
 * to the function value of the subset whose bitset is the index of the element.
 *
 * The function iterates over all subsets of each set and sums the function values, 
 * overwriting the existing vector elements with the sums.
 *
 * This function works as a utility function for the Fast Zeta Transform in Linear Space
 * algorithm.
 */
void utils::fast_down_zeta_transform_exp_space(const u_int_t& n, rval_list_t& f) {
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
	return;
}

/*
 * This is the 'up' version of the FZT. It sums for supersets instead of subsets.
 * The code is basically the same as for the 'down' version, except it counts
 * the opposite direction.
 */
void utils::fast_up_zeta_transform_exp_space(const u_int_t& n, rval_list_t& f) {	
	for (u_int_t j = U_ONE; j <= n; ++j) {
		u_int_t index = utils::exp2(n) - U_ONE;	// 2^n-1
		u_int_t step = utils::exp2(j - 1);	// 2^(j-1)
		while (index + U_ONE > U_ZERO) {
			index -= step;
			for (u_int_t i = U_ZERO; i < step; ++i) {
				add_assign(f[index], f[index + step]);
				--index;
			}
		}
	}
	return;
}

/*
 * Optimization.
 * If q == 0, 0 colourings.
 * If q == 1, 0 colourings iff |E| >= 1.
 *
 * Time complexity is (1/2)*n^2 => O(n^2) as we scan through (half of) an n*n matrix
 */
bool utils::trivial(const u_int_t& n, const u_int_t& q, bool** matrix) {
	if (q == U_ZERO) {	// 0 colourings using 0 colours.
		return true;
	} else if (q == U_ONE) {
		for (u_int_t i = U_ZERO; i != n; ++i) {
			for (u_int_t j = i + 1; j != n; ++j) {
				if ((*matrix)[i * n + j]) {
					return true;
				}
			}
		}
	}
	return false;
}


/*
 * Computes the number of ways r of colouring the graph of n vertices, represented by the
 * adjacency matrix matrix, with q colours. A pointer to a string representation of r 
 * is returned.
 */
std::string* utils::count_colourings_small_space(
		const u_int_t& n, 
		const u_int_t& q, 
		bool** matrix,
		u_int_t& nbr_procs) {

	/* Handle trivial values of q (current: 0,1,2,3) */
	/* This is done in polynomial time (generally quadratic) */
	if (trivial(n, q, matrix)) {
		return &ZERO_STR;
	}

	// partition the vertices into V1 and V2, of sizes n1 and n2 respectively
	u_int_t n1, n2;
	part_n(n, n1, n2);

//	std::cout << "n1: " << n1 << ", n2: " << n2 << std::endl;

	u_int_t two_to_the_n1 = exp2(n1);
	u_int_t two_to_the_n2 = exp2(n2);

	set_t v1 = two_to_the_n1 - 1 ;
	set_t v2 = exp2(n) - two_to_the_n1;

	set_t start = EMPTY_SET;
	set_t end = EMPTY_SET - 1;

	//std::cout << "v1 = " << v1 << std::endl;

	
	// 2. Iterate over subsets of V1 in parallel
	
	if (nbr_procs > two_to_the_n1) {
		nbr_procs = two_to_the_n1;
	}

	u_int_t slice_size = two_to_the_n1 / nbr_procs;
	rval_t* rvals = new rval_t[nbr_procs];
	thread_t* threads = new thread_t[nbr_procs];
	pth_t* pths = new pth_t[nbr_procs];
	
	for (u_int_t i = U_ZERO; i < nbr_procs; ++i) {
	//	std::cout << "starting thread loop..." << std::endl;
		init_zero(rvals[i]);
	
#ifdef PARI
		pari_thread p_th;
		u_int_t stack_size = ((PARI_STACK_CHUNK * n * n) 
				/ (nbr_procs * PARI_STACK_CHUNK)) * PARI_STACK_CHUNK;
		pari_thread_alloc(&p_th, stack_size, NULL);
		pths[i] = p_th;
#endif
		// special treatment for "last" thread, which
		// needs to take all "leftover" iterations
		// because iteration space is not an even 
		// multiple of slice_size (typically)
		if (i == nbr_procs - U_ONE) {
			start = end + U_ONE;	// one after prev threads end
			end = v1;
		} else {
			start = EMPTY_SET + slice_size * i;
			end = start + slice_size - U_ONE;
		}

		//std::cout << "Thread " << i << " started with start = " << start << 
		//	" and end = " << end << std::endl;

		threads[i] = std::thread(parallel, &rvals[i], start, end, 
				two_to_the_n1, two_to_the_n2, 
				v2, n, n2, q, matrix, &pths[i]);

	}

	for (u_int_t i = U_ZERO; i < nbr_procs; ++i) {
		threads[i].join();
	}
	delete[] threads;

	//std::cout << "If we get here thats a good thing" << std::endl;

	// 1.
	rval_t r;
	init_zero(r);

	for (u_int_t i = U_ZERO; i < nbr_procs; ++i) {
//		std::cout << rvals[i] << std::endl;
		add_assign(r, rvals[i]);
	}
	delete[] rvals;

#ifdef PARI
	for (u_int_t i = U_ZERO; i < nbr_procs; ++i) {
		pari_thread_free(&pths[i]);
	}
	delete[] pths;
#endif
	// { 3. Return the coefficient of z^n in r }}

	return print_coeff(r, n);
	
}

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
		bool** matrix,
		pth_t* pth) {

//	std::cout << "Im a thread with [start, end] = [" << start << ", " << end << "]\n";

#ifdef PARI
	pari_thread_start(pth);
#endif	
	// {{ 2. For each subset X1 of V1, do }}

	for (set_t x1 = start; x1 <= end; ++x1) {

#ifdef PARI
		pari_sp ltop = avma;
#endif

		// Data structures
		rval_list_t l(two_to_the_n2);
		rval_list_t h(two_to_the_n2);

		// {{ a) For each subset Z2 of V2, set h(Z2) <- 0 }}
		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			init_zero(h[i]);
			// We just initialize a 0-vector of size 2^n2.
			// I see no need to map these values to specific
			// indices, but instead we make sure we
			// access the proper value when using g.
		}

		// {{ b) For each subset Y1 of X1, 
		// set h(V2 \ N(Y1)) <- h(V2 \ N(Y1)) + z^(|Y1|) if Y1 independent in G }}
		for (set_t y1 = EMPTY_SET; y1 <= x1; ++y1) {
			// TODO: Fix this loop so it executes in time 2^|X1| instead of 2^c
			// where c is the position of the most significant bit of x1, or, in 
			// set terms, the highest-numbered vertex present in X1.

			if ((y1 | x1) <= x1) { // <=> Y1 is subset of X1
				// if Y1 is independent in G
				if (utils::independent(y1, matrix, n)) {
					rval_t p;
					init_monomial(size_of(y1), p);
					set_t neighbours = EMPTY_SET;
					neighbours_of(y1, matrix, n, neighbours);

					add_assign(h[(v2 & (~ neighbours)) / two_to_the_n1], p);

				}
			}
		}

		// {{ c) For each subset Y2 of V2,
		// set l(Y2) <- z^(|Y2|) if Y2 independent in G }}
		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			set_t y2 = i * two_to_the_n1;

			if (utils::independent(y2, matrix, n)) {
				init_monomial(size_of(y2), l[i]);
			} else {
				init_zero(l[i]);
			}
		}

		// {{ d) Set h <- (hS')*l }}
		utils::fast_up_zeta_transform_exp_space(n2, h);

		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			mul_assign(h[i], l[i], n);
		}
		
		// {{ e) Set h <- gS }}
		utils::fast_down_zeta_transform_exp_space(n2, h);

#ifdef PARI
		pari_sp lbot = avma;
#endif
		// {{ For each subset X2 of V2,
		// set r <- r + (-1)^(n-|X1|-|X2|) * j(X2)^k }}
		for (u_int_t i = U_ZERO; i < two_to_the_n2; ++i) {
			set_t x2 = i * two_to_the_n1;
			int exponent = n - utils::size_of(x1) - utils::size_of(x2);
			int sign = utils::exp_neg_one(exponent);

			power(h[i], q, n);
			flip_sign(h[i], sign);

#ifdef PARI
			lbot = avma;
#endif
			add_assign(*r, h[i]);
		}

#ifdef PARI
		(*r) = gerepile(ltop, lbot, *r);
#endif
		//std::cout << "I finished!" << std::endl;
	}
#ifdef PARI
	pari_thread_close();
#endif
}

/* Sets */

/*
 * Counts the size of the set set, which is written to the parameter size.
 *
 * Complexity is constant, 12 instructions for any input.
 * (source: https://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation)
 *
 */
static const u_int_t M_1 = 0x5555555555555555;	// binary 01010101...
static const u_int_t M_2 = 0x3333333333333333;	// binary 00110011...
static const u_int_t M_4 = 0x0f0f0f0f0f0f0f0f;	// binary 00001111...
static const u_int_t H01 = 0x0101010101010101;	// sum(256^i), i=0..63
u_int_t utils::size_of(const set_t& set) {
	if (N_LESS_64) {
		u_int_t size = U_ZERO;
		size = set - ((set >> 1) & M_1);	// put count of each 2 bits into those 2 bits
		size = (size & M_2) + ((size >> 2) & M_2);//put count of each 4 bits into those 4 bits 
		size = (size + (size >> 4)) & M_4;	// put count of each 8 bits into those 8 bits 
		size = (size * H01) >> 56;// sets left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ... 
		return size;
	} else {
		//TODO fix for larger int types
	}
}

/*
 * Determines if the set set is the empty set.
 *
 * Complexity is constant.
 */
bool utils::is_empty(const set_t& set) {
	return set == EMPTY_SET;
}

/*
 * Determines all neighbours of vertices in the set set. The maximum size of set is n. 
 * The result is written to the parameter neighbours.
 *
 * The function does a linear-time search to determine which vertices are in set, 
 * and then checks for every combination of the members against all other vertices.
 * Computational complexity, q is the size of set:
 *
 * O(1) + n + q*(n-1) < n + n^2 -> O(n + n^2) = O(n^2)
 *
 */
void utils::neighbours_of(const set_t& set, bool** matrix, const u_int_t& n, set_t& neighbours) {
	if (is_empty(set)) {
		neighbours = EMPTY_SET;
		return;
	}

	/* Only works if the graph is connected.
	   if (set == exp2(n) - U_ONE) {
	   neighbours = exp2(n) - U_ONE;
	   return;
	   }
	   */
	set_list_t* list = members_in(set);
	for (u_int_t i = U_ZERO; i < list->size; ++i) {
		// some iterations here are unnecessary under certain conditions.
		// If a vertex was already added to neighbours, we do not need
		// to check if it is a neighbour of also another vertex.
		// Best case performance COULD be linear, if vertex 0 is a neighbour
		// of every other vertex. This is not the current implementation.
		// TODO
		for (u_int_t j = U_ZERO; j < n; ++j) {
			if (j == i)
				continue;
			if ((*matrix)[n * (*list)[i] + j]) {
				neighbours |= exp2(j);	// 1 in pos j
			}
		}
	}
	delete list;
}

/*
 * Checks whether set is an independent set in the graph represented by the adjacency
 * matric matrix. A set is independent if for every pair of vertices v_i and v_j,
 * matrix[i][j] != 1, ie there is an edge between them. 
 *
 * Returns true iff set is independent.
 *
 * This function does a linear search to determine which vertices are in the set, and
 * then checks every combination of vertices (only once!) for an edge. Computational 
 * complexity in terms of the size q of the set and the maximum possible size n of the
 * set:
 *
 * O(1) + n + q*(q-1)/2 < n + n*(n-1)/2 = (1/2)*n^2 + (1/2)*n -> O(n^2 + n) = O(n^2) 
 *
 * If set is empty, complexity is constant.
 * 
 */
bool utils::independent(const set_t& set, bool** matrix, const u_int_t& n) {
	if (is_empty(set))
		return true;		// the empty set is always independent
	set_list_t* list = members_in(set);

	for (u_int_t i = U_ZERO; i < list->size; ++i) {
		for (u_int_t j = i + U_ONE; j < list->size; ++j) {
			if ((*matrix)[n * (*list)[i] + (*list)[j]]) {
				delete list;
				return false;
			}
		}
	}
	delete list;
	return true;
}

/*
 * Determines which vertices are present in the set set. 
 *
 * Returns a pointer to the empty list if set is the empty set. Otherwise returns a 
 * pointer to a list of the vertices present in set.
 *
 * NOTE: This method allocates dynamic memory. Remember to free this memory after 
 * you are done with it.
 *
 * This function does a constant-time check to determine the size of set, and then
 * conducts a linear search to determine which vertices are in the set set.
 * Computational complexity, r is highest index number of any vertex in set, n is
 * maximum possible size of set:
 * (The first check is to determine how much memory needs to be allocated to 
 * fit the whole list of members.)
 *
 * O(1) + O(1) + (r+1) < O(1) + O(1) + n -> O(n)
 *
 * If the set is empty, complexity is constant.
 * If the set contains vertices 0, 1, 2, ..., q, complexity is linear in q.
 */
set_list_t* utils::members_in(const set_t& set) {
	u_int_t size = size_of(set);
	set_list_t* list = new set_list_t(size);

	set_t temp = set;	
	u_int_t v = U_ZERO;
	for (u_int_t c = U_ZERO; c != size; ++v, temp = temp >> 1) {
		if (temp % 2) {
			(*list)[c] = v;
			++c;
		}
	}

	return list;
}

/* Misc. */

/*
 * Returns the xth power of -1.
 */
sign_t utils::exp_neg_one(const u_int_t& x) {
	if (N_LESS_64) {
		if ((x & U_ONE) == U_ZERO)
			return S_ONE;
		return S_N_ONE;
	} else {
		// fix for larger int types
	}
}

u_int_t utils::exp2(const u_int_t& x) {
	return U_ONE << x;
}

/*
 * This function could perform better. For an undirected graph,
 * there is no need for a complete adjacency matrix. In fact, we 
 * do not even have self-edges, so less than half of the matrix is
 * needed as output.
 */
void utils::parse(std::ifstream& infile, bool** matrix, u_int_t& n) {
	for (u_int_t i = U_ZERO; i != n; ++i) {
		for (u_int_t j = U_ZERO; j != n; ++j) {
			int v;	// this is always 0 or 1, no larger type needed
			infile >> v;
			(*matrix)[i * n + j] = v != 0;
		}
	}
}

void utils::part_n(const u_int_t& n, u_int_t& n1, u_int_t& n2) {
	if (N_LESS_64) {
		n1 = ceil(n * (log(2) / log(3)));
		n2 = n - n1;
	} else {
		// fix for larger types
	}
}

