/* file: TrainingDistributedInputId.java */
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
 * @ingroup classifier_training_distributed
 * @{
 */
package com.intel.daal.algorithms.classifier.training;

/**
 * <a name="DAAL-CLASS-ALGORITHMS__CLASSIFIER__TRAINING__TRAININGDISTRIBUTEDINPUTID"></a>
 * @brief Available identifiers of input objects of the classifier model training algorithm
 */
public final class TrainingDistributedInputId {
    private int _value;

    public TrainingDistributedInputId(int value) {
        _value = value;
    }

    public int getValue() {
        return _value;
    }

    private static final int PartialModels = 0;

    public static final TrainingDistributedInputId partialModels = new TrainingDistributedInputId(
            PartialModels); /*!< Data for the training stage */
}
/** @} */
