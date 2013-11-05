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
	public static final String PATH_TO_INPUT = "input/adjm/";
	public static final String PATH_TO_OUTPUT = "output/javatests/";

	private static class ProtocolParser {
		private Scanner scanner;
		private String cmd;
		private boolean newtest = false;

		public ProtocolParser(String filename) {
//			System.out.println(TIME);
			try {
				scanner = new Scanner(new File(filename));
			} catch (FileNotFoundException fnfe) {
				System.err.println("File not found!");
			}
			findExec();
		}

		public boolean isNewTest() {
			if (newtest) {
				newtest = false;
				return true;
			} else {
				return false;
			}
		}

		public String getTestCmdLine() {
			String line = getTestLine();
			if (line == null)
				return null;
//			System.out.println(line);
			Scanner lineScan = new Scanner(line);
			int threads = lineScan.nextInt();
			String arg = lineScan.next();
			return threads == 0 ? 
				TIME + " " + cmd + " " + PATH_TO_INPUT + arg : 
				TIME + " " + cmd + " " + PATH_TO_INPUT + arg + " " + threads;
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

		private void findExec() {
			cmd = getTestLine();
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
				"(\\d+)\\.(\\d{2})&(\\d+)\\.(\\d{2})&(\\d+)&.+_(\\d+)_(\\d+)\\s?(\\d*)");
		private FileWriter writer;
		private int fileNbr;
		private String outfileName;

		public TestResultWriter(String testfileName) throws IOException {
			outfileName = PATH_TO_OUTPUT + 
				testfileName.substring(testfileName.lastIndexOf("/") + 1);
			fileNbr = 0;
		}

		public void writeLine(InputStream is) throws IOException {
			String result = "";
			do {
				result = new BufferedReader(new InputStreamReader(is)).readLine();
				
				System.out.println("Result is: " + result);
			} while (result.startsWith("  ***   "));
			Matcher m = timePattern.matcher(result);
			if (m.find()) {
				long umillis = Long.valueOf(m.group(1)) * 1000l + 
					Long.valueOf(m.group(2)) * 10l;
				long rmillis = Integer.valueOf(m.group(3)) * 1000l +
					Long.valueOf(m.group(4)) * 10l;
				long rssmax = Integer.valueOf(m.group(5));
				int n = Integer.valueOf(m.group(6));
				int dE = Integer.valueOf(m.group(7));
				int t = 0;
				if (!m.group(8).isEmpty()) {
					t = Integer.valueOf(m.group(8));
				}
				writer.write(n + "\t" + dE + "\t" + t + "\t" + 
						umillis + "\t" + rmillis + "\t" + rssmax + "\n");
				writer.flush();
			} else {
				System.err.println("No match found! Error in pattern?");
			}
		}

		private void setTitle() throws IOException {
			writer.write("n\tdE\tt\tut\trt\trss\n");
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
		String line = null;
		while ((line = stdout.readLine()) != null) {
			System.out.println(line);
		}
	}

	public static void main(String[] args) {
		ProtocolParser pp = new ProtocolParser(args[0]);
		Runtime rt = Runtime.getRuntime();
		
		try {
			TestResultWriter trw = new TestResultWriter(args[0]);

			String cmd = null;
			while ((cmd = pp.getTestCmdLine()) != null) {
				if (pp.isNewTest()) {
					trw.setNewFile();
				}
				Process pr = rt.exec(cmd);

				// For nice output
				stdoutput(pr.getInputStream());

				try {
					pr.waitFor();
				} catch (InterruptedException e) {
					System.err.println("Don't interrupt me!");
				}

				// Read test output and write to file
				trw.writeLine(pr.getErrorStream());

			}
		} catch (IOException ioe) {
			System.err.println("IOException!");
		}
	}
}
