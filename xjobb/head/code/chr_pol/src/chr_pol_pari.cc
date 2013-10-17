#include <fstream>
#include <iostream>

#include "types.h"
#include "utils.h"
#include "interface.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc != 2)
		cout << "Usage: <prg> <infile>" << endl;

	ifstream infile(argv[1]);
	
	u_int_t n;		// Size of V

	infile >> n;

	bool* matrix = new bool[n*n];
	utils::parse(infile, &matrix, n);

	pari_init(10000000l, 2);
	
	/* Evaluate the chromatic polynomial in n+1 points */

	string** points = new string*[n + 1];
	for (u_int_t i = U_ZERO; i - 1 != n; ++i) {
		cout << "Evaluating x(" << i << ")... " << flush;
		points[i] = utils::count_colourings_small_space(n, i, &matrix);
		cout << " = " << *points[i] << endl;
	}

	/* Interpolate the evaluated points */
	interface::print_interpolate(points, n);

	cout << "Now pausing for collection of statistics. PID is " << getpid() << endl;
	cout << "Kill me with 'kill -s 9 <PID>' if necessary." << endl;
	raise(SIGSTOP);
}
