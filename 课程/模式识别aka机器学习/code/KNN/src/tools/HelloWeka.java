package tools;

import weka.core.Attribute;
import weka.core.converters.ConverterUtils.DataSource;
import weka.core.Instances;

/**
 * @author az
 */
public class HelloWeka {
    public static void main(String[] args) {
        String filePath = "C:\\Program Files\\Weka-3-8-5\\data\\weather.numeric.arff";
        try {
            // load dataset
            DataSource source = new DataSource(filePath);
            Instances instances = source.getDataSet();
            // insert attribute
            instances.insertAttributeAt(new Attribute("ctemperature"), instances.numAttributes());
            for (int i = 0; i < instances.numInstances(); i++) {
                double t = (instances.instance(i).value(1) - 32) / 1.8;
                instances.instance(i).setValue(instances.numAttributes() - 1, t);
            }
            System.out.println(instances);
        } catch (Exception e) {
        }
    }
}