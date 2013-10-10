#include <fstream>
#include <iostream>

#include "types.h"
#include "utils.h"

using namespace std;

int main(int argc, char** argv) {

	if (argc != 3)
		cout << "Usage: <prg> <infile> <k>" << endl;

	ifstream infile(argv[1]);
	
	u_int_t n;		// Size of V
	u_int_t k;		// Number of colors to use.

	infile >> n;
	k = atoi(argv[2]);

	bool* matrix = new bool[n*n];
	utils::parse(infile, &matrix, n);

	pari_init(10000000000l, 2);

	utils::small_space_chr_pol(n, k, &matrix);

	cout << "Now pausing for collection of statistics. PID is " << getpid() << endl;
	cout << "Kill me with 'kill -s 9 <PID>' if necessary." << endl;
	raise(SIGSTOP);
}
