/* file: eltwise_sum_layer_dense_batch.cpp */
/*******************************************************************************
* Copyright 2014-2017 Intel Corporation
* All Rights Reserved.
*
* If this  software was obtained  under the  Intel Simplified  Software License,
* the following terms apply:
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*
*
* If this  software  was obtained  under the  Apache License,  Version  2.0 (the
* "License"), the following terms apply:
*
* You may  not use this  file except  in compliance  with  the License.  You may
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*
* Unless  required  by   applicable  law  or  agreed  to  in  writing,  software
* distributed under the License  is distributed  on an  "AS IS"  BASIS,  WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
* See the   License  for the   specific  language   governing   permissions  and
* limitations under the License.
*******************************************************************************/

/*
!  Content:
!    C++ example of forward and backward element-wise sum layer usage
!
!******************************************************************************/

/**
 * <a name="DAAL-EXAMPLE-CPP-ELTWISE_SUM_LAYER_BATCH"></a>
 * \example eltwise_sum_layer_dense_batch.cpp
 */

#include "daal.h"
#include "service.h"

using namespace std;
using namespace daal;
using namespace daal::algorithms;
using namespace daal::algorithms::neural_networks::layers;
using namespace daal::data_management;
using namespace daal::services;

/* Input data set parameters */
string datasetName = "../data/batch/layer.csv";

/* Number of input tensors */
const size_t nInputs = 3;

int main()
{
    /* Create an algorithm to compute forward element-wise sum layer results using default method */
    eltwise_sum::forward::Batch<> eltwiseSumLayerForward;

    /* Read datasetFileName from a file and create a tensor to store input data */
    for (size_t i = 0; i < nInputs; i++)
    {
        TensorPtr tensorData = readTensorFromCSV(datasetName);

        /* Set input objects for the forward element-wise sum layer */
        eltwiseSumLayerForward.input.set(forward::inputLayerData, tensorData, i);
    }

    /* Compute forward element-wise sum layer results */
    eltwiseSumLayerForward.compute();

    /* Print the results of the forward element-wise sum layer */
    eltwise_sum::forward::ResultPtr forwardResult = eltwiseSumLayerForward.getResult();
    printTensor(forwardResult->get(forward::value), "Forward element-wise sum layer result (first 5 rows):", 5);
    printNumericTable(forwardResult->get(eltwise_sum::auxNumberOfCoefficients),
        "Forward element-wise sum layer number of inputs (number of coefficients)", 1);

    /* Create an algorithm to compute backward element-wise sum layer results using default method */
    eltwise_sum::backward::Batch<> eltwiseSumLayerBackward;

    /* Set input objects for the backward element-wise sum layer */
    eltwiseSumLayerBackward.input.set(backward::inputGradient, readTensorFromCSV(datasetName));
    eltwiseSumLayerBackward.input.set(backward::inputFromForward, forwardResult->get(forward::resultForBackward));

    /* Compute backward element-wise sum layer results */
    eltwiseSumLayerBackward.compute();

    /* Print the results of the backward element-wise sum layer */
    eltwise_sum::backward::ResultPtr backwardResult = eltwiseSumLayerBackward.getResult();

    for (size_t i = 0; i < nInputs; i++)
    {
        printTensor(backwardResult->get(backward::resultLayerData, i),
            "Backward element-wise sum layer backward result (first 5 rows):", 5);
    }

    return 0;
}
