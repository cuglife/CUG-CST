package tools;

import weka.core.Instance;

/**
 * @author az
 */
public class Neighbor {
    public Instance instance;
    public Double distance;

    public Neighbor(Instance instance, Double distance) {
        this.instance = instance;
        this.distance = distance;
    }

    public void setDistance(Double distance) {
        this.distance = distance;
    }

    public void setInstance(Instance instance) {
        this.instance = instance;
    }
}