package tools;

import weka.core.Instances;
import weka.filters.Filter;
import weka.filters.unsupervised.attribute.Standardize;
import weka.filters.unsupervised.attribute.Normalize;

/**
 * @author az
 */
public class FeatureScaler {
    /**
     * Returns a scaled version (using standardized normalization) of the given dataset.
     *
     * @param instances The original dataset.
     * @return A scaled instances object.
     */
    public Instances scaleData(Instances instances) throws Exception {
        Standardize standardize = new Standardize();
        standardize.setInputFormat(instances);
        return Filter.useFilter(instances, standardize);
    }

    public Instances scaleDataNormalize(Instances instances) throws Exception {
        Normalize normalize = new Normalize();
        normalize.setInputFormat(instances);
        return Filter.useFilter(instances, normalize);
    }
}
