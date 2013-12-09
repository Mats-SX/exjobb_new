package src;

import java.util.HashMap;
import java.util.HashSet;

public class VertexSet {
	private HashMap<Integer, Vertex> vSet;

	/**
	 * Creates empty vertex set
	 */
	public VertexSet() {
		vSet = new HashMap<Integer, Vertex>();
	}

	/**
	 * Copy constructor for safety
	 */
	public VertexSet(VertexSet other) {
		this.vSet = new HashMap<Integer, Vertex>(other.vSet);
	}

	/**
	 * Adds element to this set.
	 * Returns true if this set was changed as a result of this
	 * operation.
	 * If element is null, always returns false.
	 */
	public boolean add(Vertex element) {
		if (element == null) {
			return false;
		}
		return vSet.put(element.getId(), element) == null;
	}


	/**
	 * Returns a neighbour of v from this set.
	 * If no neighbour of v exists in this set,
	 * returns null
	 */
	public Vertex getNeighbourOf(Vertex v) {
		for (Integer key : vSet.keySet()) {
			Vertex w = vSet.get(key);
			if (w.isNeighbourOf(v)) {
				return w;
			}
		}
		return null;
	}

	/**
	 * Returns true if v is in this set
	 */
	public boolean contains(Vertex v) {
		return vSet.containsKey(v.getId());
	}

	/**
	 * Returns true if a neighbour of v exists in this set
	 */
	public boolean containsNeighbourOf(Vertex v) {
		return getNeighbourOf(v) != null;
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("VertexSet = { ");
		for (Integer i : vSet.keySet()) {
			sb.append(vSet.get(i));
			sb.append(" ,");
		}
		sb.append("}");
		return sb.toString();
	}


	public static class Vertex {
		private static int ID_COUNTER = 1;
		private final int id;
		//private final int degree;
		private final HashSet<Vertex> neighbours;
		private int colour;
		
		public void setColour(int c) {
			colour = c;
		}
		
		public int getColour() {
			return colour;
		}

		public Vertex() {
			this.id = ID_COUNTER++;
		//	degree = 0;
			neighbours = new HashSet<Vertex>();
		}

		public void addNeighbour(Vertex v) {
			neighbours.add(v);
		//	degree = neighbours.size();
		}

		public HashSet<Vertex> getNeighbours() {
			return new HashSet<Vertex>(neighbours);
		}

		public boolean isNeighbourOf(Vertex v) {
			return neighbours.contains(v);
		}

		public int getId() {
			return id;
		}

		@Override
		public int hashCode() {
			int hash = 1;
			hash = hash * 17 + id;
		//	hash = hash * 31 + degree;
			return hash;
		}

		@Override
		public boolean equals(Object other) {
			if (other == this) {
				return true;
			} else if (other == null) {
				return false;
			} else if (other instanceof Vertex) {
				Vertex o = (Vertex) other;
				return this.id == o.id;
			} else {
				return false;
			}
		}

		public String toString() {
			return "Vertex " + String.valueOf(id);
		}
	}
}
