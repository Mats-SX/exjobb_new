import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

public class Parser {

	static Pattern fstrow = Pattern.compile(".+_(\\d{1,2})_(\\d{1,3}) (\\d{1,3})");
	static Pattern sndrow = Pattern.compile("ut, st, tt: (\\d+) (\\d+) (\\d+)");
	static Pattern thdrow = Pattern.compile("vm, rss: (\\d+) (\\d+)");

	public static void main(String[] args) throws IOException {
		Scanner scan = null;
		try {
			scan = new Scanner(new FileReader(args[0]));
		} catch (FileNotFoundException e) {

		}

		System.out.println("Hello world!");

		System.out.println("Now we're reading from " + args[0]);

		// Remove dir path and trailing "_res"
		String problemName = args[0].substring(7);
		problemName = problemName.substring(0, problemName.length() - 4);
		
		String timeFile1, timeFile2, timeFile3, memFile1, memFile2, memFile3;
		timeFile1 = "output/parsed_results/" + problemName + "_time1";
		timeFile2 = "output/parsed_results/" + problemName + "_time2";
		timeFile3 = "output/parsed_results/" + problemName + "_time3";
		memFile1 = "output/parsed_results/" + problemName + "_mem1";
		memFile2 = "output/parsed_results/" + problemName + "_mem2";
		memFile3 = "output/parsed_results/" + problemName + "_mem3";

		FileWriter fw_t1 = new FileWriter(timeFile1);
		FileWriter fw_t2 = new FileWriter(timeFile2);
		FileWriter fw_t3 = new FileWriter(timeFile3);
		FileWriter fw_m1 = new FileWriter(memFile1);
		FileWriter fw_m2 = new FileWriter(memFile2);
		FileWriter fw_m3 = new FileWriter(memFile3);

		fw_t1.write("n\tut\tst\ttt\n");
		fw_t2.write("n\tut\tst\ttt\n");
		fw_t3.write("df\tut\tst\ttte\n");
		fw_m1.write("n\tvm\trss\n");
		fw_m2.write("n\tvm\trss\n");
		fw_m3.write("df\tvm\trss\n");

		/* Parse test1 */
		while (!scan.hasNext("@@@")) {
			scanAndWrite(fw_t1, fw_m1, scan, Test.T);
		}
		scan.nextLine();
		fw_t1.flush();
		fw_m1.flush();
		fw_t1.close();
		fw_m1.close();

		while (!scan.hasNext("@@@")) {
			scanAndWrite(fw_t2, fw_m2, scan, Test.T);
		}
		scan.nextLine();
		fw_t2.flush();
		fw_t2.close();
		fw_m2.flush();
		fw_m2.close();

		while (!scan.hasNext("@@@")) {
			scanAndWrite(fw_t3, fw_m3, scan, Test.DF);
		}
		scan.nextLine();
		fw_t3.flush();
		fw_t3.close();
		fw_m3.flush();
		fw_m3.close();
	}

	public static enum Test {
		DF,
		N,
		T;
	}

	public static void scanAndWrite(FileWriter fw1,	
			FileWriter fw2, Scanner scan, Test t) throws IOException {
		String line = scan.nextLine();
		//System.out.println(line);
		Matcher m = fstrow.matcher(line);
		m.find();
		String n = m.group(1);
		String df = m.group(2);
		String T = m.group(3);
		m = sndrow.matcher(scan.nextLine());
		m.find();
		String ut = m.group(1);
		String st = m.group(2);
		String tt = m.group(3);
		m = thdrow.matcher(scan.nextLine());
		m.find();
		String vm = m.group(1);
		String rss = m.group(2);
		
		String head = "";
		switch (t) {
			case DF:
				head = df;
				break;
			case N:
				head = n;
				break;
			case T:
				head = T;
				break;
		}

		fw1.write(head + "\t" + ut + "\t" + st + "\t" + tt + "\n");
		fw2.write(head + "\t" + vm + "\t" + rss + "\n");
	}
}
