package tools;

import Normal.KNN;
import weka.core.Instance;

/**
 * DistanceCalculator
 *
 * @author az
 */
public class DistanceCalculator {

    private double p;
    private boolean calculateEfficient;
    private double cutOfValue;

    public DistanceCalculator(double p, KNN.DistanceCheck distanceCheck) {
        this.p = p;
        this.calculateEfficient = distanceCheck == KNN.DistanceCheck.Efficient;
        this.cutOfValue = Double.MAX_VALUE;
    }

    public void setCutOfValue(double cutOfValue) {
        this.cutOfValue = cutOfValue;
    }

    /**
     * We leave it up to you whether you want the distance method to get all relevant
     * parameters(lp, efficient, etc..) or have it has a class variables.
     */
    public double distance(Instance one, Instance two) {
        if (p == Double.MAX_VALUE) {
            return lInfinityDistance(one, two);
        } else {
            return lpDistance(one, two);
        }
    }

    /**
     * Returns the Lp distance between 2 instances.
     */
    private double lpDistance(Instance one, Instance two) {
        int n = one.numAttributes() - 1;
        double sum = 0.0;
        double poweredCutOfValue = 1.0;
        if (this.calculateEfficient) {
            poweredCutOfValue = Math.pow(this.cutOfValue, p);
        }

        for (int i = 0; i < n; i++) {
            sum += Math.pow(Math.abs(one.value(i) - two.value(i)), this.p);
            if (this.calculateEfficient && sum >= poweredCutOfValue) {
                return Double.MAX_VALUE;
            }
        }
        return Math.pow(sum, 1 / this.p);
    }

    /**
     * Returns the L infinity distance between 2 instances.
     */
    private double lInfinityDistance(Instance one, Instance two) {
        int n = one.numAttributes() - 1;
        double absMax = 0.0;
        double current;

        for (int i = 0; i < n; i++) {
            current = Math.abs(one.value(i) - two.value(i));
            if (current > absMax) {
                absMax = current;
                if (this.calculateEfficient && absMax >= this.cutOfValue) {
                    return Double.MAX_VALUE;
                }
            }
        }
        return absMax;
    }

    /**
     * Returns the Lp distance between 2 instances, while using an efficient distance check.
     */
    private double efficientLpDistance(Instance one, Instance two) {
        int n = one.numAttributes() - 1;
        double sum = 0.0;

        for (int i = 0; i < n; i++) {
            sum += Math.pow(Math.abs(one.value(i) - two.value(i)), this.p);
            if (sum >= this.cutOfValue) {
                return Double.MAX_VALUE;
            }
        }
        return Math.pow(sum, 1 / this.p);
    }

}
