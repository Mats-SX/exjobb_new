#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <string>
#include <gmpxx.h>
#include <gmp.h>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;

/*
 * Expected input: ifname
 * ifname: Name of input file.
 */
int main(int argc, char** argv) {

	cout << "Instance Creator for Graph coloring problems." << endl;

	if (argc < 2) {
		cout	<< "Too few arguments." << endl
			<< "Usage: m_gen <ifname>" << endl
			<< "Now exiting." << endl;
		return 0;
	}

	ifstream infile(argv[1]);

	string line;
	do {
		getline(infile, line);
	} while (line[0] == '#');	// skip comments

	istringstream iss(line);

	int nbr_of_rows, inst;
	iss >> nbr_of_rows;
	iss >> inst;

	for (int i = 0; i < nbr_of_rows; ++i) {

		do {
			getline(infile, line);
		} while (line[0] == '#');	// skip comments

		iss.str(line);
		iss.clear();

		int alpha, n, type;
		iss >> n;
		iss >> alpha;
		iss >> type;

		cout << "Input read: n = " << n << ", alpha = " << alpha << endl;
		cout << "Generating " << inst << " such graphs of type " << type << ".\n";

		gmp_randstate_t state;
		gmp_randinit_default(state);
		
		for (int k = 0; k < inst; ++k) {

			mpz_t m, m_max, m_cur, r;		// m is #edges in this graph
			mpz_inits(m, m_max, m_cur, r, NULL);	// m_max is max #edges
			// m_cur is a gmp counter
			// m_max = n(n-1)/2
			mpz_set_ui(m_max, n);			// m_max = n
			mpz_mul_ui(m_max, m_max, n - 1);	// m_max *= n-1
			mpz_cdiv_q_ui(m_max, m_max, 2);		// m_max /= 2

			if (type == -1) { // old behaviour
				// m = (alpha/100)*m_max
				mpz_mul_ui(m, m_max, alpha);		// m = m_max*alpha
				mpz_cdiv_q_ui(m, m, 100);		// m /= 100
			} else {
				double t = 0;
				if (type == 0) {
					// quadratic increase in edges
					t = alpha * n * n;
				} else if (type == 1) {
					// linear increase in edges
					t = alpha * n;
				} else if (type == 2) {
					// loglin increase in edges
					t = alpha * log(n) * n;
				} else if (type == 3) {
					// quadoverlog increase in edges
					t = alpha * n * n / log(n);
				}
				mpz_set_ui(m, static_cast<unsigned long>(t));
				mpz_cdiv_q_ui(m, m, 100);
			}

			//		mpz_set(m_cur, m_max);			// m_cur = m_max

			bool* matrix = new bool[n * n];

			vector<bool> distribution;		// word with m bits set
			// holds m_max bits
			for (; mpz_cmp(m, m_cur) > 0; mpz_add_ui(m_cur, m_cur, 1)) {
				distribution.push_back(1);	// set m bits
			}
			for (; mpz_cmp(m_max, m_cur) > 0; mpz_add_ui(m_cur, m_cur, 1)) {
				distribution.push_back(0);	// unset m_max-m bits
			}

			if (!mpz_fits_ulong_p(m_max)) {
				cout << "Too many edges. Unsupported!" << endl;
				return 0;
			}

			unsigned long int dist_size = distribution.size(); 

			// Shuffle the distribution using Fisher–Yates shuffle
			for (unsigned long int i = dist_size - 1; i > 0; --i) {
				unsigned long int j = rand() % (i + 1);			
				bool b = distribution[i];
				distribution[i] = distribution[j];
				distribution[j] = b;
			}


			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					int index = i * n + j;
					if (i == j)
						matrix[index] = 0;
					else if (j < i)
						matrix[index] = matrix[j * n + i];
					else {	// the loop will fall here m_max times.
						matrix[index] = distribution.back();
						distribution.pop_back();
					}
				}
			}

			ostringstream outname;
			outname << "input/adjm/paper/";
			if (type == 0) {
				outname << "quad/";
			} else if (type == 1) {
				outname << "linear/";
			} else if (type == 2) {
				outname << "loglin/";
			} else if (type == 3) {
				outname << "quadoverlog/";
			}
			outname << "g_" << n << "_" << alpha << "-" << k;

			ofstream outfile(outname.str().c_str());

			outfile << n << " " << mpz_get_ui(m) << "\n";
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					outfile << matrix[i * n + j];
					outfile << " ";
				}
				outfile << "\n";
			}	
			cout << "A matrix is now stored in " << outname.str() << endl;
		}
	}

	return 0;
}
