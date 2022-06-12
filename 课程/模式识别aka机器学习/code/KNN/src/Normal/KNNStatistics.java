package Normal;

import tools.FeatureScaler;
import weka.core.Instance;
import weka.core.Instances;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;


/**
 * @author az
 */
public class KNNStatistics {

    public static BufferedReader readDataFile(String filename) {
        BufferedReader inputReader = null;
        try {
            inputReader = new BufferedReader(new FileReader(filename));
        } catch (FileNotFoundException ex) {
            System.err.println("File not found: " + filename);
        }
        return inputReader;
    }

    public static Instances loadData(String fileName) throws IOException {
        BufferedReader datafile = readDataFile(fileName);
        Instances data = new Instances(datafile);
        data.setClassIndex(data.numAttributes() - 1);
        return data;
    }

    /**
     * Finds the best hyper parameters (K – number of neighbors, Lp distance(Minkowski distance) measure, weighting scheme)
     * using 10-folds cross validation, for a given data set.
     *
     * @param maxK - try k's in range [1..maxK]
     * @param setP - a given set of values
     * @return [k, p, prediction type (0 - uniform, 1 - weighted), avg error]
     */
    static double[] searchHyperParameters(int maxK, double[] setP, Instances data) throws Exception {
        // init some values
        KNN knn = new KNN();
        knn.buildClassifier(data);
        double currentError;
        double[] bestHyperParameters = new double[4];
        bestHyperParameters[3] = Double.MAX_VALUE;

        // Iterate for looking best K
        for (int k = 1; k <= maxK; k++) {
            knn.setK(k);
            for (double p : setP) {
                knn.setP(p);
                for (KNN.WeightType weightType : KNN.WeightType.values()) {
                    knn.setPredictionType(weightType);
                    currentError = knn.calculateCrossValidationError(data, 10);
                    if (currentError < bestHyperParameters[3]) {
                        bestHyperParameters[0] = k;
                        bestHyperParameters[1] = p;
                        bestHyperParameters[2] = (weightType == KNN.WeightType.Uniform) ? 0 : 1;
                        bestHyperParameters[3] = currentError;
                    }

                }
            }
        }
        return bestHyperParameters;
    }

    static void appendSectorsCrossValidation(StringBuilder results, Instances randomizedScaledData,
                                             int[] setFolds, double[] bestParamScaled) throws Exception {
        KNN knn = new KNN();
        knn.buildClassifier(randomizedScaledData);
        knn.setK((int) bestParamScaled[0]);
        knn.setP(bestParamScaled[1]);
        knn.setPredictionType((bestParamScaled[2] == 0) ?
                KNN.WeightType.Uniform : KNN.WeightType.Weighted);
        knn.setIfMeasureTime(true);
        long predictionRunTime;
        double error;
        String dc;

        for (int folds : setFolds) {
            results.append("----------------------------\n")
                    .append("Results for ").append(folds).append(" folds: \n");
            for (KNN.DistanceCheck distanceCheck : KNN.DistanceCheck.values()) {
                dc = (distanceCheck == KNN.DistanceCheck.Regular) ? "regular" : "efficient";
                knn.setDistanceCheck(distanceCheck);
                error = knn.calculateCrossValidationError(randomizedScaledData, folds);
                predictionRunTime = knn.getPredictionRunTime();
                results.append("Cross validation error of ").append(dc)
                        .append(" knn on auto_price dataset is ").append(error)
                        .append("\nThe average elapsed time is: ").append(predictionRunTime / folds).append(" ns")
                        .append("\nThe total elapsed time is: ").append(predictionRunTime).append(" ns\n\n");
            }
        }
    }

    static void appendSectorBestParams(StringBuilder results,
                                       double[] bestParamUnscaled, double[] bestParamScaled) {
        results.append("----------------------------\n")
                .append("Results for original dataset: \n")
                .append("Cross validation error with K = ").append((int) bestParamUnscaled[0])
                .append(", lp = ").append((int) bestParamUnscaled[1])
                .append(", majority function = ").append((bestParamUnscaled[2] == 0) ? "uniform" : "weighted")
                .append(",\nfor auto_price data is: ").append(bestParamUnscaled[3])

                .append("\n\n").append("----------------------------\n")
                .append("Results for scaled dataset: \n")
                .append("Cross validation error with K = ").append((int) bestParamScaled[0])
                .append(", lp = ").append((int) bestParamScaled[1])
                .append(", majority function = ").append((bestParamScaled[2] == 0) ? "uniform" : "weighted")
                .append(",\nfor auto_price data is: ").append(bestParamScaled[3]).append("\n\n");
    }

    public static void printInstances(Instances instances) {
        try {
            for (Instance instance : instances) {
                System.out.println(instance);
            }
        } catch (Exception e) {
        }
    }


    public static void main(String[] args) throws Exception {
        // load data and generate random data
        String fileName = "data/auto_price.txt";
        Instances data = loadData(fileName);
        Instances randomizedData = loadData(fileName);
        randomizedData.randomize(new java.util.Random(4));
        // data scale for better performance
        Instances randomizedScaledData = new FeatureScaler().scaleData(randomizedData);

//         System.out.println("randomizedData");
//         printInstances(randomizedData);
//         System.out.println("randomizedScaledData");
//         printInstances(randomizedScaledData);

        StringBuilder results = new StringBuilder();

        // 最大 K 取值
        int maxK = 20;
        // 距离度量公式选择
        double[] setP = {1, 2, Double.MAX_VALUE};
        // K 折验证
        int[] setFolds = {data.numInstances(), 50, 10, 5, 3};

        // get best Param
        // returns [k, p, prediction type (0 - uniform, 1 - weighted), avg error]
        double[] bestParamUnscaled = searchHyperParameters(maxK, setP, randomizedData);
        double[] bestParamScaled = searchHyperParameters(maxK, setP, randomizedScaledData);

        // print algorithm performance 组织输出结果
        appendSectorBestParams(results, bestParamUnscaled, bestParamScaled);
        appendSectorsCrossValidation(results, randomizedScaledData, setFolds, bestParamScaled);
        System.out.println(results);
    }

}
