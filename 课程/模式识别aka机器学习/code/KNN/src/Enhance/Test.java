package Enhance;

import Normal.KNN;
import Normal.KNNStatistics;
import tools.CorrelationCalculator;
import tools.EnhanceDistanceCalculator;
import tools.FeatureScaler;
import tools.Neighbor;
import weka.core.Instance;
import weka.core.Instances;

/**
 * @author az
 */
public class Test {

    public static double[] runKNNTest(Instances data, int K, double P, EnhanceKNN.WeightType weightType, int[] setFolds) throws Exception {
        EnhanceKNN knn = new EnhanceKNN();
        knn.buildClassifier(data);
        double currentError;
        double[] hyperParameters = new double[4];
        hyperParameters[3] = Double.MAX_VALUE;

        double[] correlations = CorrelationCalculator.calculate(data);

        knn.setCorrelations(correlations);
        knn.setK(K);
        knn.setP(P);
        knn.setPredictionType(weightType);
        currentError = knn.calculateCrossValidationError(data, 10);
        if (currentError < hyperParameters[3]) {
            hyperParameters[0] = K;
            hyperParameters[1] = P;
            hyperParameters[2] = (weightType == EnhanceKNN.WeightType.Uniform) ? 0 : 1;
            hyperParameters[3] = currentError;
        }
        return hyperParameters;
    }

    public static void printResult(double[] hyperParameters) {
        StringBuilder results = new StringBuilder();
        results.append("----------------------------\n")
                .append("Results for dataset: \n")
                .append("Cross validation error with K = ").append((int) hyperParameters[0])
                .append(", lp = ").append((int) hyperParameters[1])
                .append(", majority function = ").append((hyperParameters[2] == 0) ? "uniform" : "weighted")
                .append(",\nfor auto_price data is: ").append(hyperParameters[3]);
        System.out.println(results);
    }

    public static void main(String[] args) throws Exception {
        // load data and generate random data
        String fileName = "data/auto_price.txt";
        Instances data = KNNStatistics.loadData(fileName);

        Instances randomizedData = KNNStatistics.loadData(fileName);
        randomizedData.randomize(new java.util.Random(4));
        // data scale for better performance
        Instances randomizedScaledData = new FeatureScaler().scaleDataNormalize(randomizedData);




//        System.out.println("randomizedData");
//        KNNStatistics.printInstances(randomizedData);
//        System.out.println("randomizedScaledData");
//        KNNStatistics.printInstances(randomizedScaledData);


//        int K = 4;
//        double P = 1;
//        // KNN.WeightType weightType = KNN.WeightType.Uniform;
//        EnhanceKNN.WeightType weightType = EnhanceKNN.WeightType.Weighted;
//        int[] setFolds = {data.numInstances(), 50, 10, 5, 3};
//
//        double[] hyperParameters = runKNNTest(randomizedScaledData, K, P, weightType, setFolds);
//
//        // print algorithm performance 组织输出结果
//        printResult(hyperParameters);
        double[] correlations = CorrelationCalculator.calculate(data);
        EnhanceDistanceCalculator distanceCalculator = new EnhanceDistanceCalculator(1, EnhanceKNN.DistanceCheck.Regular, correlations);

        System.out.println(data.instance(148).value(0));
        System.out.println(data.instance(149).value(0));
        System.out.println(data.instance(120).value(0));
        System.out.println(data.instance(122).value(0));
        System.out.println(data.instance(126).value(0));
        System.out.println(data.instance(90).value(0));

        Instance instance1 = data.instance(1);
        Instance instance2 = data.instance(2);
        System.out.println(instance1);
        System.out.println(instance1.value(0));
        System.out.println(instance2);
        System.out.println(instance2.value(0));

        double distance = distanceCalculator.distance(instance1, instance2);

        System.out.println(distance);

    }

}
