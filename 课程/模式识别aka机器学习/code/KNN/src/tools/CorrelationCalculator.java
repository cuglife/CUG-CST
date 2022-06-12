package tools;

import weka.core.Instances;

/**
 * @author az
 */
public class CorrelationCalculator {
    public static double[] getColumn(Instances instances, int valueIndex) {
        int instancesNum = instances.numInstances();
        double[] columnData = new double[instancesNum];
        for (int instanceIndex = 0; instanceIndex < instancesNum; instanceIndex++) {
            columnData[instanceIndex] = Double.valueOf(instances.instance(instanceIndex).value(valueIndex));
        }
        return columnData;
    }


    public static double getCorrelation(double X[], double Y[]) {
        if (X.length != Y.length) {
            throw new RuntimeException("data error!!");
        }
        int n = X.length;

        double sum_X = 0, sum_Y = 0, sum_XY = 0;
        double squareSum_X = 0, squareSum_Y = 0;

        for (int i = 0; i < n; i++) {
            // sum of elements of array X.
            sum_X = sum_X + X[i];

            // sum of elements of array Y.
            sum_Y = sum_Y + Y[i];

            // sum of X[i] * Y[i].
            sum_XY = sum_XY + X[i] * Y[i];

            // sum of square of array elements.
            squareSum_X = squareSum_X + X[i] * X[i];
            squareSum_Y = squareSum_Y + Y[i] * Y[i];
        }

        // use formula for calculating correlation
        // coefficient.
        double corr = (float) (n * sum_XY - sum_X * sum_Y) /
                (float) (Math.sqrt((n * squareSum_X - sum_X * sum_X)
                        * (n * squareSum_Y - sum_Y * sum_Y)));

        return corr;
    }

    public static double[] calculate(Instances instances) {
        int instancesNum = instances.numInstances();
        int num = instances.numAttributes() - 1;
        double[] correlations = new double[num];
        double[] lastData = getColumn(instances, num);
        for (int i = 0; i < num; i++) {
            double[] tmpData = getColumn(instances, i);
            double correlation = getCorrelation(tmpData, lastData);
            correlations[i] = correlation;
        }
        return correlations;
    }
}
