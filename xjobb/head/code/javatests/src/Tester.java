import java.util.StringTokenizer;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.File;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.FileWriter;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class Tester {
	public static final String TIME = "/usr/bin/time --format=\"%U&%e&%M&%C\"";
	public static String PATH_TO_INPUT = "input/adjm/hundred/";
	public static final String PATH_TO_OUTPUT = "output/javatests/";
	public static boolean tutte = false;

	private static class ProtocolParser {
		private Scanner scanner;
		private String cmd;
		private int nbrTests;
		private int curTest;
		private String lastArg;
		private int threads;
		private boolean newtest = false;

		public ProtocolParser(String filename) {
			//			System.out.println(TIME);
			try {
				scanner = new Scanner(new File(filename));
			} catch (FileNotFoundException fnfe) {
				System.err.println("File not found!");
			}
			init();
		}

		public boolean isNewTest() {
			if (newtest) {
				newtest = false;
				return true;
			} else {
				return false;
			}
		}

		public int iters() {
			return nbrTests;
		}

		public String getTestCmdLine() {
			String retCmd = "";
			if (curTest < nbrTests) {
				retCmd = lastArg + "-" + curTest;
				++curTest;
			} else {
				String line = getTestLine();
				if (line == null)
					return null;
				//System.out.println(line);
				Scanner lineScan = new Scanner(line);
				threads = lineScan.nextInt();
				String arg = lineScan.next();
				lastArg = TIME + " " + cmd + " " + PATH_TO_INPUT + arg;
				curTest = 0;
				return getTestCmdLine();
			}
			return threads == 0 ? retCmd : retCmd + " " + threads;
		}

		private String getTestLine() {
			String line = scanner.nextLine();
			if (line.startsWith("#")) {		// comments
				return getTestLine();
			} else if (line.startsWith("@")) {	// test markers
				newtest = true;
				return getTestLine();
			} else if (line.startsWith("$")) {	// end-of-protocol
				return null;
			}
			return line;
		}

		private void init() {
			String line = getTestLine();
			cmd = line.substring(0, line.lastIndexOf(" "));
			nbrTests = Integer.valueOf(line.substring(line.lastIndexOf(" ") + 1));
			curTest = nbrTests;
		}

	}

	/**
	 * This class reads output from the program /usr/bin/time
	 * and prints it to file in a format readible by pgfplots in latex
	 */
	private static class TestResultWriter {
		/*
		 * Groups:
		 * 1. user time seconds
		 * 2. user time centiseconds
		 * 3. real time seconds
		 * 4. real time centiseconds
		 * 5. peak resident set size
		 * 6. n
		 * 7. dE
		 * 8. t
		 */
		public static final Pattern timePattern = Pattern.compile(
		"(\\d+)\\.(\\d{2})&(\\d+)\\.(\\d{2})&(\\d+)&.+_(\\d+)_(\\d+)-\\d{0,3}\\s?(\\d*)");
		private FileWriter writer;
		private int fileNbr;
		private final String outfileName;
		private final int datapoints;
		private int curPoints;
		private long umillis;
		private long rmillis;
		private long rssmax;
		private int n;
		private int dE;
		private int t;

		public TestResultWriter(String testfileName, int d) throws IOException {
			outfileName = PATH_TO_OUTPUT + 
				testfileName.substring(testfileName.lastIndexOf("/") + 1);
			fileNbr = 0;
			curPoints = 0;
			datapoints = d;
		}

		public void parseLine(InputStream is) throws IOException {
			String result = "";
			do {
				result = new BufferedReader(new InputStreamReader(is)).readLine();

				System.out.println("Result is: " + result);
			} while (result.startsWith("  ***   "));
			Matcher m = timePattern.matcher(result);
			if (m.find()) {
				++curPoints;
				long _umillis = Long.valueOf(m.group(1)) * 1000l + 
					Long.valueOf(m.group(2)) * 10l;
				long _rmillis = Integer.valueOf(m.group(3)) * 1000l +
					Long.valueOf(m.group(4)) * 10l;
				long _rssmax = Integer.valueOf(m.group(5));
				/* Take average */
				umillis += _umillis / datapoints;
				rmillis += _rmillis / datapoints;
				rssmax += _rssmax / datapoints;
				n = Integer.valueOf(m.group(6));
				dE = Integer.valueOf(m.group(7));
				if (tutte) {
					// dE is really m. need to convert
					dE *= 100;
					dE = dE / ((n * (n-1)) / 2);
				}
				t = 0;
				if (!m.group(8).isEmpty()) {
					t = Integer.valueOf(m.group(8));
				}
			} else {
				System.err.println("No match found! Error in pattern?");
			}
			if (curPoints == datapoints) {
				setNewRow();
				curPoints = 0;
			}
		}

		private void setTitle() throws IOException {
			writer.write("n\tdE\tt\tut\trt\trss\n");
		}

		private void setNewRow() throws IOException {
			writer.write(n + "\t" + dE + "\t" + t + "\t" + 
					umillis + "\t" + rmillis + "\t" + rssmax + "\n");
			writer.flush();
			umillis = 0;
			rmillis = 0;
			rssmax = 0;
		}

		public void setNewFile() throws IOException {
			if (writer != null)
				writer.close();
			++fileNbr;
			writer = new FileWriter(outfileName + "" + fileNbr);
			setTitle();
		}
	}

	private static void stdoutput(InputStream is) throws IOException {

		BufferedReader stdout = new BufferedReader(
				new InputStreamReader(is));
		int len = 10;
		char[] buf = new char[len];
		String line = null;
		int read = 0;
		while (stdout.ready()) {
			read = stdout.read(buf, read, 10);
			if (read == len) {
				System.out.println(buf);
				read = 0;
			}
		}
	}

	public static void main(String[] args) {
		boolean silent = false;
		if (args.length > 1 && args[1].equals("-q")) {
			silent = true;
		}
		if (args.length > 2 && args[2].equals("tutte")) {
			tutte = true;
			PATH_TO_INPUT = "input/edgelists/hundred/";
		}
		ProtocolParser pp = new ProtocolParser(args[0]);
		Runtime rt = Runtime.getRuntime();

		try {
			TestResultWriter trw = new TestResultWriter(args[0], pp.iters());

			String cmd = null;
			while ((cmd = pp.getTestCmdLine()) != null) {
				if (pp.isNewTest()) {
					trw.setNewFile();
				}

//				System.out.println("Running cmd: " + cmd);
				Process pr = rt.exec(cmd);

				if (!silent) {
					// For nice output
					stdoutput(pr.getInputStream());
				}

				try {
					pr.waitFor();
				} catch (InterruptedException e) {
					System.err.println("Don't interrupt me!");
				}

				// Read test output and write to file
				trw.parseLine(pr.getErrorStream());

			}
		} catch (IOException ioe) {
			ioe.printStackTrace();
			System.err.println("IOException!");
		}
	}
}
