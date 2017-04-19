/* file: TrainingResult.java */
/*******************************************************************************
* Copyright 2014-2017 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
 * @ingroup brownboost_training
 * @{
 */
package com.intel.daal.algorithms.brownboost.training;

import com.intel.daal.algorithms.ComputeMode;
import com.intel.daal.algorithms.brownboost.Model;
import com.intel.daal.algorithms.classifier.training.TrainingResultId;
import com.intel.daal.services.DaalContext;

/**
 * <a name="DAAL-CLASS-ALGORITHMS__BROWNBOOST__TRAINING__TRAININGRESULT"></a>
 * @brief Provides methods to access final results obtained with the compute() method of the BrownBoost training algorithm
 *                                                                                                in the batch processing mode
 */
public final class TrainingResult extends com.intel.daal.algorithms.boosting.training.TrainingResult {
    /** @private */
    static {
        System.loadLibrary("JavaAPI");
    }

    public TrainingResult(DaalContext context, long cObject) {
        super(context, cObject);
    }

    /**
     * Returns the final result of the BrownBoost training algorithm
     * @param id   Identifier of the result
     * @return         %Result that corresponds to the given identifier
     */
    @Override
    public Model get(TrainingResultId id) {
        if (id != TrainingResultId.model) {
            throw new IllegalArgumentException("type unsupported");
        }
        return new Model(getContext(), cGetModel(cObject, TrainingResultId.model.getValue()));
    }
}
/** @} */
