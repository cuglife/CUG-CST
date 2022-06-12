package tools;

import java.util.Comparator;

/**
 * @author az
 */
public class NeighborComparator implements Comparator<Neighbor> {

    @Override
    public int compare(Neighbor a, Neighbor b) {
        return b.distance.compareTo(a.distance);
    }
}
