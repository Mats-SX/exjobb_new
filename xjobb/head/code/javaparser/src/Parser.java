import java.util.Scanner;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

public class Parser {

	public static void main(String[] args) throws IOException {
		Scanner scan = null;
		try {
			scan = new Scanner(new FileReader(args[0]));
		} catch (FileNotFoundException e) {

		}
	
		System.out.println("Hello world!");

		System.out.println("Now we're reading from " + args[0]);

		// Remove dir path and trailing "_test_res"
		String problemName = args[0].substring(12);
		problemName = problemName.substring(0, problemName.length() - 9);
		
		String timeFile1, timeFile2, timeFile3, memFile1, memFile2, memFile3;
		timeFile1 = problemName + "_time1";
		timeFile2 = problemName + "_time2";
		timeFile3 = problemName + "_time3";
		memFile1 = problemName + "_mem1";
		memFile2 = problemName + "_mem2";
		memFile3 = problemName + "_mem3";

		FileWriter fw_t1 = new FileWriter(timeFile1);
		FileWriter fw_t2 = new FileWriter(timeFile2);
		FileWriter fw_t3 = new FileWriter(timeFile3);
		FileWriter fw_m1 = new FileWriter(memFile1);
		FileWriter fw_m2 = new FileWriter(memFile2);
		FileWriter fw_m3 = new FileWriter(memFile3);

		fw_t1.write("df\ttime\tsystime\n");
		fw_t2.write("n\ttime\tsystime\n");
		fw_t3.write("k\ttime\tsystime\n");
		fw_m1.write("df\tvm\trss\n");
		fw_m2.write("n\tvm\trss\n");
		fw_m3.write("k\tvm\trss\n");

		/* Parse test1 */
		for (int i = 0; i < 21; ++i) {
			String df = scan.next().substring(6);
			scan.nextLine();	// not interested in k
			scan.skip("ut, st, tt:");
			String ut = scan.next();
			String st = scan.next();
			scan.nextLine();	// total time not interesting...
			scan.skip("vm, rss:");
			String vm = scan.next();
			String rss = scan.next();
			scan.nextLine();	// skip newline
			
			fw_t1.write(df + "\t" + ut + "\t" + st + "\n");
			fw_m1.write(df + "\t" + vm + "\t" + rss + "\n");
		}
		fw_t1.flush();
		fw_m1.flush();
		fw_t1.close();
		fw_m1.close();

		for (int i = 0; i < 8; ++i) {
			String n = scan.next().substring(3).substring(0, 2);
			scan.nextLine();	// not interested in k
			fw_t2.write(n + "\t");
			fw_m2.write(n + "\t");
			scanAndWrite(fw_t2, fw_m2, scan);
		}
		fw_t2.flush();
		fw_t2.close();
		fw_m2.flush();
		fw_m2.close();

		for (int i = 0; i < 98; ++i) {
			scan.next();
			String k = scan.next();
			scan.nextLine();
			fw_t3.write(k + "\t");
			fw_m3.write(k + "\t");
			scanAndWrite(fw_t3, fw_m3, scan);
		}
		fw_t3.flush();
		fw_t3.close();
		fw_m3.flush();
		fw_m3.close();
	}

	public static void scanAndWrite(FileWriter fw1,	
			FileWriter fw2, Scanner scan) throws IOException {
		scan.skip("ut, st, tt:");
		String ut = scan.next();
		String st = scan.next();
		scan.nextLine();
		scan.skip("vm, rss:");
		String vm = scan.next();
		String rss = scan.next();
		scan.nextLine();
		
		fw1.write(ut + "\t" + st + "\n");
		fw2.write(vm + "\t" + rss + "\n");
	}
}
