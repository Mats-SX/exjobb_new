import java.util.Scanner;
import java.io.FileWriter;
import java.io.FileReader;

public class EdgeListGen {

	/**
	 * args[0] needs to be a filename of a file containing an adjacency matrix
	 * OR
	 * if args[1] == 'd', we will read all files in a directory. We assumed they
	 * are called args[0]-i, with i in [0,99]
	 */
	public static void main(String[] args) throws Exception {
		int limit = 1;
		boolean dir = false;
		if (args.length > 1 && args[1].equals("d")) {
			// directory scan
			limit = Integer.valueOf(args[2]);
			dir = true;
		}

		for (int k = 0; k < limit; ++k) {
			String infile = dir ? args[0] + "-" + k : args[0];
			Scanner scan = new Scanner(new FileReader(infile));
			System.out.println("Reading an adjancency matrix from " + infile);
			int n = scan.nextInt();

			String outname = "input/edgelists/";
			if (dir) {
				outname += "hundred/";
			}
			outname += "el_" + String.valueOf(n);

			StringBuilder sb = new StringBuilder();
			scan.nextLine();			// this also skips m
								// backwards compatibility!
			int nbrEdges = 0;
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					if (j <= i) {
						scan.nextInt();
						continue;
					}
					int b = scan.nextInt();
					if (b != 0) {
						sb.append(i).
							append("--").
							append(j).
							append(",");
						nbrEdges++;
					}
				}
				scan.nextLine();
			}

			sb.deleteCharAt(sb.length() - 1);	// remove last comma
			outname = outname + "_" + nbrEdges;
			if (dir) {
				outname += "-" + k;
			}

			FileWriter fp = new FileWriter(outname);
			fp.write(sb.toString());
			fp.flush();
			fp.close();

			System.out.println("Done converting " + infile + " to an edge list " +
					outname + ".");
		}
	}
}
