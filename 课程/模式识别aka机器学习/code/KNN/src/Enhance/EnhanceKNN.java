package Enhance;

import tools.*;
import weka.classifiers.Classifier;
import weka.core.Capabilities;
import weka.core.Instance;
import weka.core.Instances;
import weka.filters.Filter;
import weka.filters.supervised.instance.StratifiedRemoveFolds;

import java.util.*;

/**
 * @author az
 */
public class EnhanceKNN implements Classifier {

    public enum DistanceCheck {
        /**
         * Regular,
         */
        Regular,
        /**
         * Efficient,
         */
        Efficient
    }

    /**
     * weights type
     */
    public enum WeightType {
        /**
         * Uniform,  all neighbors get an equally weighted “vote”
         */
        Uniform,
        /**
         * Weighted, weigh each observation’s “vote” by its distance from the observation
         */
        Weighted
    }

    private Instances trainingInstances;
    private int k;
    /**
     * Minkowski distance param, p>=1
     * p = 1 Minkowski distance trans to Manhattan distance
     * p = 2 Minkowski distance trans to Euclidean distance
     * p = ∞ Minkowski distance trans to Chebyshev distance
     */
    private double p;
    private DistanceCheck distanceCheck;
    private WeightType weightType;
    private boolean ifMeasureTime;
    private long predictionRunTime;
    private double[] correlations;

    /**
     * Build the knn classifier. In our case, simply stores the given instances for
     * later use in the prediction.
     */
    @Override
    public void buildClassifier(Instances instances) {
        this.trainingInstances = instances;
        this.weightType = WeightType.Uniform;
        this.distanceCheck = DistanceCheck.Regular;
        this.ifMeasureTime = false;
    }

    public void setK(int k) {
        this.k = k;
    }

    public void setP(double p) {
        this.p = p;
    }

    public void setCorrelations(double[] correlations) {
        this.correlations = correlations;
    }

    public void setDistanceCheck(DistanceCheck distanceCheck) {
        this.distanceCheck = distanceCheck;
    }

    public void setPredictionType(WeightType weightType) {
        this.weightType = weightType;
    }

    public void setIfMeasureTime(boolean ifMeasureTime) {
        this.ifMeasureTime = ifMeasureTime;
    }

    public long getPredictionRunTime() {
        return predictionRunTime;
    }

    /**
     * meat of the function
     * Returns the knn prediction on the given instance.
     *
     * @return The instance predicted value.
     */
    public double regressionPrediction(Instance instance) {
        double prediction;
        PriorityQueue<Neighbor> kNN = findNearestNeighbors(instance);
        if (this.weightType == WeightType.Weighted) {
            prediction = getWeightedAverageValue(kNN);
        } else {
            prediction = getAverageValue(kNN);
        }

        return prediction;
    }

    /**
     * Calculate the average error on a give set of instances.
     * The average error is the average absolute error between the target value and the predicted
     * value across all instances.
     */
    public double calculateAverageError(Instances instances) {
        double diff, prediction;
        double sumOfAbsoluteErrors = 0.0;

        for (Instance instance : instances) {
            prediction = regressionPrediction(instance);
            diff = instance.value(instance.classIndex()) - prediction;
            sumOfAbsoluteErrors += Math.abs(diff);
        }

        return sumOfAbsoluteErrors / instances.numInstances();
    }

    /**
     * Calculates the cross validation error, the average error on all folds.
     *
     * @param instances instances used for the cross validation
     * @param foldsNum  The number of folds to use.
     * @return The cross validation error.
     */
    public double calculateCrossValidationError(Instances instances, int foldsNum) throws Exception {
        StratifiedRemoveFolds foldsFilter = new StratifiedRemoveFolds();
        foldsFilter.setNumFolds(foldsNum);
        Instances initialClassMember = this.trainingInstances;
        Instances validationFold;
        double sumOfFoldsErrors = 0.0;

        long predictionClock = 0;
        if (this.ifMeasureTime) {
            this.predictionRunTime = 0;
        }

        for (int i = 1; i <= foldsNum; i++) {
            // focus on fold 'i'
            foldsFilter.setFold(i);

            // set the rest of the data ("training data")
            foldsFilter.setInvertSelection(true);
            foldsFilter.setInputFormat(instances);
            this.trainingInstances = Filter.useFilter(instances, foldsFilter);

            // set the validation data part
            foldsFilter.setInvertSelection(false);
            foldsFilter.setInputFormat(instances);
            validationFold = Filter.useFilter(instances, foldsFilter);

            // measure time if requested
            if (this.ifMeasureTime) {
                predictionClock = System.nanoTime();
            }

            // Calculate the error of the fold, considering the rest of the data
            sumOfFoldsErrors += calculateAverageError(validationFold);

            // measure time if requested
            if (this.ifMeasureTime) {
                predictionClock = System.nanoTime() - predictionClock;
                this.predictionRunTime += predictionClock;
            }
        }
        // return the class member to initial value
        this.trainingInstances = initialClassMember;

        return sumOfFoldsErrors / foldsNum;
    }

    /**
     * Finds the k nearest neighbors.
     */
    public PriorityQueue<Neighbor> findNearestNeighbors(Instance instance) {
        PriorityQueue<Neighbor> kNearestNeighborsQueue = new PriorityQueue<>(new NeighborComparator());
        EnhanceDistanceCalculator distanceCalculator = new EnhanceDistanceCalculator(this.p, this.distanceCheck, this.correlations);

        double distance;
        Neighbor furthest, neighbor;

        // adds all first k instances to the mapping
        for (int i = 0; i < k; i++) {
            distance = distanceCalculator.distance(instance, this.trainingInstances.instance(i));
            neighbor = new Neighbor(this.trainingInstances.instance(i), distance);
            kNearestNeighborsQueue.add(neighbor);
        }

        furthest = kNearestNeighborsQueue.peek();
        assert furthest != null;
        distanceCalculator.setCutOfValue(furthest.distance);

        // replace the furthest neighbor if necessary
        for (int i = k; i < this.trainingInstances.numInstances(); i++) {
            distance = distanceCalculator.distance(instance, this.trainingInstances.instance(i));

            if (distance < furthest.distance) {
                kNearestNeighborsQueue.remove(furthest);
                neighbor = new Neighbor(this.trainingInstances.instance(i), distance);
                kNearestNeighborsQueue.add(neighbor);
                furthest = kNearestNeighborsQueue.peek();
                assert furthest != null;
                distanceCalculator.setCutOfValue(furthest.distance);
            }
        }

        return kNearestNeighborsQueue;
    }


    /**
     * Calculate the average value of the given elements in the collection.
     */
    public double getAverageValue(PriorityQueue<Neighbor> neighbors) {
        double sum = 0.0;
        for (Neighbor neighbor : neighbors) {
            sum += neighbor.instance.value(neighbor.instance.classIndex());
        }
        return sum / neighbors.size();
    }

    /**
     * Calculates the weighted average of the target values of all the elements in the collection
     * with respect to their distance from a specific instance.
     * weight = 1 / distance^2
     */
    public double getWeightedAverageValue(PriorityQueue<Neighbor> neighbors) {
        double weight, value, distance;
        double sumOfWeights = 0.0;
        double sumOfWeightedValues = 0.0;

        for (Neighbor neighbor : neighbors) {
            distance = neighbor.distance;
            value = neighbor.instance.value(neighbor.instance.classIndex());
            if (distance == 0) {
                return value;
            }
            weight = 1 / Math.pow(distance, 2);

            sumOfWeights += weight;
            sumOfWeightedValues += weight * value;
        }

        return sumOfWeightedValues / sumOfWeights;
    }


    @Override
    public double[] distributionForInstance(Instance arg0) {
        // TODO Auto-generated method stub - You can ignore.
        return null;
    }

    @Override
    public Capabilities getCapabilities() {
        // TODO Auto-generated method stub - You can ignore.
        return null;
    }

    @Override
    public double classifyInstance(Instance instance) {
        // TODO Auto-generated method stub - You can ignore.
        return 0.0;
    }
}
