import java.util.Scanner;
import java.io.FileWriter;
import java.io.FileReader;

public class EdgeListGen {

	/**
	 * args[0] needs to be a filename of a file containing an adjacency matrix
	 */
	public static void main(String[] args) throws Exception {
		Scanner scan = new Scanner(new FileReader(args[0]));
		System.out.println("Reading an adjancency matrix from " + args[0]);
		int n = scan.nextInt();
		String outname = "input/edgelists/el_" + String.valueOf(n);
		StringBuilder sb = new StringBuilder();

		scan.nextLine();
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
		
		FileWriter fp = new FileWriter(outname);
		fp.write(sb.toString());
		fp.flush();
		fp.close();

		System.out.println("Done converting " + args[0] + " to an edge list " +
				outname + ".");
	}
}
