#include <fstream>
#include <iostream>

#include "types.h"
#include "utils.h"
#include "interface.h"

using namespace std;

int main(int argc, char** argv) {

	u_int_t q = -1;
	if (argc == 4) {
		q = atoi(argv[3]);
	} else if (argc != 3)
		cout << "Usage: <prg> <infile> <threads> [q]" << endl;

	ifstream infile(argv[1]);
	
	u_int_t n;		// Size of V
	u_int_t m;		// Size of E
	u_int_t threads;	// Parallelization factor

	infile >> n;
	infile >> m;
	threads = atoi(argv[2]);

	bool* matrix = new bool[n*n];
	utils::parse(infile, &matrix, n);

	u_int_t stack_size = PARI_STACK_CHUNK * n;

	pari_init(stack_size, 0);

	if (q == -1) {
		/* Evaluate the chromatic polynomial in n+1 points */

		string** points = new string*[n + 1];
		for (u_int_t i = U_ZERO; i - 1 != n; ++i) {
			cout << "Evaluating x(" << i << ")... " << flush;
			points[i] = utils::count_colourings_small_space(n, m, i, &matrix, threads);
			cout << " = " << *points[i] << endl;
		}

		/* Interpolate the evaluated points */
		interface::print_interpolate(points, n);

		delete[] points;
	} else {
		/* Evaluate only at one point */
		/* This is for the decision problem */

		string* s = utils::count_colourings_small_space(n, m, q, &matrix, threads);
		cout << *s << endl;
	}
	delete[] matrix;


	pari_close();

}
