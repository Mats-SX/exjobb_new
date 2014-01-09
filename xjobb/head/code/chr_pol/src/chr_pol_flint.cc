#include <fstream>
#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "types.h"
#include "utils.h"
#include "interface.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc != 3) {
		cout << "Usage: <prg> <infile> <threads>" << endl;
		return 1;
	}

	ifstream infile(argv[1]);
	
	u_int_t n;		// Size of V
	u_int_t m;		// Size of E
	u_int_t threads;	// Parallelization factor

	infile >> n;
	infile >> m;
	threads = atoi(argv[2]);

	bool* matrix = new bool[n*n];
	utils::parse(infile, &matrix, n);

	/* Evaluate the chromatic polynomial in n+1 points */

	string** points = new string*[n + 1];
	for (u_int_t i = U_ZERO; i - 1 != n; ++i) {
		cout << "Evaluating x(" << i << ")... " << flush;
		points[i] = utils::count_colourings_small_space(n, m, i, &matrix, threads);
		cout << " = " << *points[i] << endl;
	}

	/* Interpolate the evaluated points */
	interface::print_interpolate(points, n);

}
