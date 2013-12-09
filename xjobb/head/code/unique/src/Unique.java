package src;

import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import src.VertexSet.Vertex;

public class Unique {

	public static void main(String[] args) {
		/*
		 * Read input
		 * args[0] <- name of file containing adjacency matrix
		 */
		
		String infile = args[0];

		ArrayList<Vertex> V = new ArrayList<Vertex>();
		parse(infile, V);


		UniqueColouring uc = new UniqueColouring(V);
//		uc.init();

		if (uc.colour()) {
			System.out.println(infile + " contains a uniquely 3-colourable graph!");
			System.out.println("The coloring is as follows:");
			uc.printColouring();
		} else {
			System.out.println(infile + " does not contain a uniquely 3-colourable graph!");
		}
		uc.printMetrics();
	}

	/**
	 * Parses an adjacency matrix located in the file filename
	 * and puts all vertices into the list V.
	 */
	public static void parse(String filename, List<Vertex> V) {
		Scanner fileScanner = null;
		try {
			fileScanner = new Scanner(new File(filename));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			System.exit(1);
		}

		System.out.println("Reading a graph from " + filename);

		int n = fileScanner.nextInt();
		int m = fileScanner.nextInt();
		fileScanner.nextLine();

		System.out.println("n, m = " + n + ", " + m);

		/* Create n vertices */
		/* Each vertex will end up on position i, where i is also their id */
		// I don't seem to make use of this fact ...
		for (int i = 0; i < n; ++i) {
			V.add(new Vertex());
		}

		/* Scan the adjacency matrix and add edges */
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				int edge = fileScanner.nextInt();
				if (edge == 1) {
					V.get(i).addNeighbour(V.get(j));
				}
			}
		}

		fileScanner.close();
	}	
}
