#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <ios>
#include <string>
#include <cstring>
#include <limits>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes five doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(
		const pid_t& pid,
		unsigned long& vm_usage,
		unsigned long& resident_set,
		double& user_time,
		double& system_time,
		double& total_time)
{
	using std::ios_base;
	using std::ifstream;
	using std::string;

	vm_usage	= 0;
	resident_set	= 0;
	user_time	= 0.0;
	system_time	= 0.0;
	total_time	= 0.0;

	// 'file' stat seems to give the most reliable results
	//
	stringstream s1;
	s1 << "/proc/" << pid << "/status";
	
	ifstream status_stream(s1.str().c_str(), ios_base::in);

	stringstream s2;
	s2 << "/proc/" << pid << "/stat";
	ifstream stat_stream(s2.str().c_str(), ios_base::in);

	// dummy vars for leading entries in stat that we don't care about
	
	string pid2, comm, state, ppid, pgrp, session, tty_nr;
	string tpgid, flags, minflt, cminflt, majflt, cmajflt;
	string cutim, cstime, priority, nice, num_threads, itrealvalue;

	// the fields we want
	//
	unsigned long starttime;
	unsigned long vsize;
	unsigned long rss;
	unsigned long utime;
	unsigned long stime;

	string line;
	string junk;
	for (int i = 0; i < 11; ++i)
		getline(status_stream, line);
	istringstream iss(line);
	iss >> junk;
	iss >> vsize;
	for (int i = 0; i < 4; ++i)
		getline(status_stream, line);
	iss.str(line);
	iss.clear();
	iss >> junk;
	iss >> rss;

	
	stat_stream >> pid2 >> comm >> state >> ppid >> pgrp >> session >> tty_nr
	       	>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
		>> utime >> stime >> cutime >> cstime >> priority >> nice
		>> num_threads >> itrealvalue >> starttime; // don't care about the rest
	

	stat_stream.close();

//	long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
	vm_usage	= vsize;		// kB
	resident_set	= rss;			// kB
	user_time	= (1000.0 * utime) / (static_cast<double>(sysconf(_SC_CLK_TCK)));	// milliseconds
	system_time	= (1000.0 * stime) / (static_cast<double>(sysconf(_SC_CLK_TCK)));	// milliseconds
	total_time	= user_time + system_time;
}


/*
 * Input file format:
 * <k> <nbr of values to follow> <file#1> <file#2> ... <file#n>
 * ...
 *
 *
 */
int main(int argc, char** argv) {

	cout << "This is autotesting." << endl;

	ifstream infile(argv[1]);	// Read what to test from file
	
	string line;
	do {
		getline(infile, line);
	} while (line[0] == '#');

	istringstream iss(line);

	// I have no idea why, but declaring this where I want to declare it 
	// causes really strange errors....
	string arg; 
	
	string protocol_name(argv[1]);
	int pos = protocol_name.rfind("/");
	protocol_name = protocol_name.substr(pos + 1);

	string prg;
	iss >> prg;

	ostringstream ss;
	ss << "output/" << prg.substr(5) << "_" << protocol_name << "_res";
	ofstream out(ss.str().c_str());

	while (true) {
		do {
			getline(infile, line);
		} while (line[0] == '#');
		
		// end of this test
		if (line == "@@@") {
			out << "@@@" << endl;
			continue;
		}

		// end of test protocol file
		if (line == "@@@@") {
			break;
		}

		iss.str(line);
		iss.seekg(0);

		int nbr_of_tests;
		iss >> nbr_of_tests;

		int threads;
		iss >> threads;

		for (int l = 0; l < nbr_of_tests; ++l) {
			iss >> arg;
			
			out << arg << " " << threads << endl;
			
			stringstream cmd;
			cmd 	<< prg << " input/"
				<< arg << " " << threads;

			cout << "Now running: " << cmd.str() << endl;

			char* args[4];
			char tmp[35];
		       	strcpy(tmp, cmd.str().c_str());
			
			args[0] = strtok(tmp, " ");
			for (int i = 1; i < 4; ++i) {
				args[i] = strtok(NULL, " ");
			}

			pid_t pid;
			switch((pid = fork())) {
				case -1:
				/* Fork failed */
				cerr << "Failed fork!" << endl;
				return 2;
				break;

				case 0:
				/* Let the child execute the test */
				execv(prg.c_str(), args);
				/* We will never get here (hopefully) */
				break;
			}

			/* Wait for child to finish before starting next */
			waitpid(pid, NULL, WUNTRACED);
			
			/* Measure child performance */
			double ut, st, tt;
			unsigned long vm, rss;
			process_mem_usage(pid, vm, rss, ut, st, tt);
			
			unsigned long _ut = static_cast<unsigned long>(ut);
			unsigned long _tt = static_cast<unsigned long>(tt);

			/* Print stats to stdout */
			cout	<< "User time used:\t\t" << (_ut) << " ms" 	<< endl
				<< "System time used:\t" << (st) << " ms"	<< endl
				<< "Total time used:\t" << (_tt) << " ms"	<< endl
				<< "Virtual Memory:\t\t" << vm << " KB"		<< endl
				<< "Resident set size:\t" << rss << " KB"	<< endl;
			
			/* Print stats to outfile */
			out	<< "ut, st, tt: " << (_ut) << " " << st << " " << (_tt) << endl;
			out	<< "vm, rss: " << vm << " " << rss << endl;

			/* Let child terminate (to release memory) */
			kill(pid, SIGCONT);

			/* Let's proceed to next test. */
			
			cout << "#####################" << endl;
		}
	}
	out.flush();
	out.close();

	cout << "Output written to " << ss.str() << endl;

	//execl("/usr/bin/mv", "/usr/bin/mv", ss.str().c_str(), "output", NULL);

	return 0;
}
