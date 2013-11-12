import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class DimacsToMatrix {

	private static final Pattern edgePattern = Pattern.compile("e (\\d+) (\\d+)");

	public static void main(String[] args) throws FileNotFoundException, IOException { 

		Scanner scan = new Scanner(new File(args[0]));
		
		System.out.println("Now reading from " + args[0]);

		String line = "";
		do {
			line = scan.nextLine();
		} while (line.startsWith("c"));

		Scanner s2 = new Scanner(line);

		s2.next();		// p
		s2.next();		// edge
		int n = s2.nextInt();
		int m = s2.nextInt();
		int[][] matrix = new int[n][n];

		while (scan.hasNext()) {

			Matcher match = edgePattern.matcher(scan.nextLine());
			if (match.find()) {
				int v = Integer.valueOf(match.group(1)) - 1;// we index from 0, not 1
				int w = Integer.valueOf(match.group(2)) - 1;
				matrix[v][w] = 1;
				matrix[w][v] = 1;
			}
		}

		double m_max = (n - 1) * n / 2.0;
		double dE_ = (m / m_max) * 100.0;

		int dE = (int) Math.ceil(dE_);

		String outname = "input/adjm/dim_" + n + "_" + dE;

		System.out.println("Writing a matrix to " + outname);

		FileWriter fw = new FileWriter(outname);
		fw.write(n + " " + m + "\n");
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				fw.write(String.valueOf(matrix[i][j]) + " ");
			}
			fw.write("\n");
		}
		fw.flush();
		fw.close();
	}

}
