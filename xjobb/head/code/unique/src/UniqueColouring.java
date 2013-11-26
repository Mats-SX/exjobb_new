package src;

import java.util.ArrayList;
import src.VertexSet.Vertex;

public class UniqueColouring {
	private VertexSet[] s;
	private ArrayList<Vertex> V;
	private int simpleSet;
	private UniqueColouring solution;
	private static long LOOPS = 0;
	private static long SIMPLE = 0;
	private static long HARD = 0;

	public UniqueColouring(ArrayList<Vertex> V) {
		s = new VertexSet[3];
		s[0] = new VertexSet();
		s[1] = new VertexSet();
		s[2] = new VertexSet();
		this.V = V;
		simpleSet = -1;
	}

	private UniqueColouring(ArrayList<Vertex> V,
			VertexSet s1,
			VertexSet s2,
			VertexSet s3) {
		this.V = V;
		this.s = new VertexSet[3];
		s[0] = new VertexSet(s1);
		s[1] = new VertexSet(s2);
		s[2] = new VertexSet(s3);
		simpleSet = -1;
	}

	/** Sets the first two vertices into s1 and s2.
	 * This is always correct behaviour.
	 */
	public void init() {
		Vertex v0 = V.remove(0);
		Vertex v1 = v0.getNeighbours().iterator().next();
		s[0].add(v0);
		s[1].add(v1);
		V.remove(v0);
		V.remove(v1);

//		System.out.println("Initialized a unique colouring.");
//		System.out.println("Picked initial vertices v0, v1 = " + v0 + ", " + v1);
	}

	private Vertex nextUnassignedNeighbour() {
		for (int j = 0; j < V.size(); ++j) {
			if (isAssigned(V.get(j)))
				continue;
			for (int i = 0; i < 3; ++i) {
				if (s[i].containsNeighbourOf(V.get(j))) {
					simpleSet = i;
					return V.get(j);
				}
			}
		}
		return null;
	}

	private ReturnValue simpleCase(Vertex w, int otherSet) {
		++SIMPLE;
	
		/* Easy: if w has two assigned neighbours in different sets */
		/* One is already known to be assigned in otherSet */
		/* Then we assign w to the only available set */
		for (int i = 0; i < 3; ++i) {
			if (i == otherSet)
				continue;
			if (s[i].containsNeighbourOf(w)) {
				int j = 3 - i - otherSet;
				if (s[j].containsNeighbourOf(w)) {
					// This is a wrong colouring branch!
					return ReturnValue.UNCOLOURABLE; 
				}
				s[j].add(w);
				V.remove(w);

//				System.out.println("Simple case succeeded! " + 
//						w + " was put in set " + j);
				return ReturnValue.SUCCESS;
			}
		}
//		System.out.println("Simple case failed for " + w);
		return ReturnValue.FAILURE;
	}

	private boolean hardCase(Vertex w, int otherSet) {
		++HARD;
		for (Vertex u : w.getNeighbours()) {
			if (isAssigned(u)) {
				continue;
			}
			for (int i = 0; i < 3; ++i) {
				if (i == otherSet)
					continue;
				if (s[i].containsNeighbourOf(u)) {
					
					// Make two recursive calls here
					// Either u belongs in otherSet
					// OR
					// u belongs in third set (which is !i and !otherSet)
					// AND
					// w belongs in second set (i)
					//
					// If our problem here is T(n),
					// one recursive branch is T(n-1)
					// and the other is T(n-2),
					// yielding T(n) = T(n-1) + T(n-2)
					// => O(2^(n/2)) (I think)
					//
					int j = 3 - i - otherSet;
					
					ArrayList<Vertex> v2 = new ArrayList<Vertex>(V);
					v2.remove(u);
					v2.remove(w);
					VertexSet s1 = new VertexSet(s[i]);
					VertexSet s2 = new VertexSet(s[j]);
					
					s1.add(w);
					s2.add(u);
					
					UniqueColouring fst = new UniqueColouring(
							v2, s1, s2, s[otherSet]);
					
					if (fst.colour()) {
						if (fst.solution != null)
							solution = fst.solution;
						else
							solution = fst;
						return true;
					}
					
					ArrayList<Vertex> v1 = new ArrayList<Vertex>(V);
					v1.remove(u);
					s1 = new VertexSet(s[otherSet]);
					s1.add(u);
					UniqueColouring snd = new UniqueColouring(
							v1, s1, s[i], s[j]);
					if (snd.colour()) {
						if (snd.solution != null)
							solution = snd.solution;
						else
							solution = snd;
						return true;
					}
				}
			}
		}
		return false;
	}
	
	private boolean isAssigned(Vertex v) {
		for (int i = 0; i < 3; ++i) {
			if (s[i].contains(v)) {
				return true;
			}
		}
		return false;
	}

	public boolean colour() {
		++LOOPS;
		ReturnValue rv = null;
		
		int maxLoops = V.size();
		int curLoops = 0;
		
		Vertex first = null;
		
		while (true) {
			
			if (V.isEmpty())
				/* We have successfully assigned all vertices */
				return true;			
			
			/* Pick an unassigned vertex, adjacent to any assigned vertex */
			Vertex w = nextUnassignedNeighbour();
			
			if (w == null) {
				/* 
				 * There were no unassigned neighbours available
				 * This graph is not uniquely 3-colourable
				 */
				return false;
			}

			/* 
			 * Test if w has two assigned neighbours in different sets.
			 * If yes, assign it.
			 */
			rv = simpleCase(w, simpleSet);
			
			/*
			 * If we assigned w, repeat.
			 * If we did not assign w, repeat with another vertex.
			 * If we have tried all adjacent vertices, proceed.
			 * If w can not be assigned, the graph is not uniquely 3-colourable
			 */
			switch (rv) {
			case SUCCESS:
				first = null;
				continue;
			case FAILURE:
				if (first == null)
					first = w;
				else if (first.equals(w)) {
					first = null;
					break;
				}
				V.remove(w);
				V.add(w);
				continue;
			case UNCOLOURABLE:
				return false;
			}

			/* 
			 * We can't know the colour of any vertex,
			 * but we do know that only two possibilities exist.
			 * We call them recursively.
			 */
			
			if (hardCase(w, simpleSet)) {
				return true;
			} else {
				/*
				 * Both recursions failed, we need to start over with
				 * another start vertex
				 */
				V.remove(w);
				V.add(w);
				++curLoops;
				if (curLoops > maxLoops) {
					/* 
					 * We have tried all possible start vertices
					 * and all recursions failed. This graph is not
					 * uniquely 3-colourable.
					 */
					return false;
				}	
			}
		}
	}

	public void printColouring() {
		if (solution == null) {
			solution = this;
		}
		System.out.println(solution.s[0]);
		System.out.println(solution.s[1]);
		System.out.println(solution.s[2]);
		
		System.out.println("Metrics:\tloops\tsimple\thard");
		System.out.println("\t\t" + LOOPS + "\t" + SIMPLE + "\t" + HARD);
	}
	
	private enum ReturnValue {
		SUCCESS,
		FAILURE,
		UNCOLOURABLE;
	}
}
