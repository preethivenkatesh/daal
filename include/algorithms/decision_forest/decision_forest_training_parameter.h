/* file: decision_forest_training_parameter.h */
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
//++
//  Decision forest training parameter class
//--
*/

#ifndef __DECISION_FOREST_TRAINING_PARAMETER_H__
#define __DECISION_FOREST_TRAINING_PARAMETER_H__

#include "algorithms/algorithm.h"
#include "data_management/data/numeric_table.h"
#include "data_management/data/data_serialize.h"
#include "services/daal_defines.h"
#include "algorithms/engines/mt19937/mt19937.h"

namespace daal
{
namespace algorithms
{
/**
 * @defgroup base_decision_forest Base Decision Forest
 * \brief Contains base classes of the decision forest algorithm
 * @ingroup training_and_prediction
 */
/**
 * \brief Contains classes of the decision forest algorithm
 */
namespace decision_forest
{
/**
 * \brief Contains a class for decision forest model-based training
 */
namespace training
{
/**
 * @ingroup base_decision_forest
 * @{
 */
/**
 * <a name="DAAL-ENUM-ALGORITHMS__DECISION_FOREST__TRAINING__VARIABLE_IMPORTANCE_MODE"></a>
 * \brief Variable importance computation mode
 */
enum VariableImportanceMode
{
    none,           /* Do not compute */
    MDI,            /* Mean Decrease Impurity.
                       Computed as the sum of weighted impurity decreases for all nodes where the variable is used,
                       averaged over all trees in the forest */
    MDA_Raw,        /* Mean Decrease Accuracy (permutation importance).
                       For each tree, the prediction error on the out-of-bag portion of the data is computed
                       (error rate for classification, MSE for regression).
                       The same is done after permuting each predictor variable.
                       The difference between the two are then averaged over all trees. */
    MDA_Scaled      /* Mean Decrease Accuracy (permutation importance).
                       This is MDA_Raw value scaled by its standard deviation. */
};

/**
 * <a name="DAAL-ENUM-ALGORITHMS__DECISION_FOREST__TRAINING__RESULTTOCOMPUTEID"></a>
 * Available identifiers to specify the result to compute
 */
enum ResultToComputeId
{
    computeOutOfBagError = 0x00000001ULL
};

/**
 * \brief Contains version 1.0 of the Intel(R) Data Analytics Acceleration Library (Intel(R) DAAL) interface
 */
namespace interface1
{

/**
 * <a name="DAAL-CLASS-ALGORITHMS__DECISION_FOREST__TRAINING__PARAMETER"></a>
 * \brief Parameters for the decision forest algorithm
 *
 * \snippet decision_forest/decision_forest_training_parameter.h Parameter source code
 */
/* [Parameter source code] */
class DAAL_EXPORT Parameter
{
public:
    Parameter() : nTrees(100),
        observationsPerTreeFraction(1.),
        featuresPerNode(0), maxTreeDepth(0),
        minObservationsInLeafNode(0),
        seed(77),
        impurityThreshold(0.),
        varImportance(none),
        resultsToCompute(0),
        memorySavingMode(false),
        bootstrap(true),
        engine(engines::mt19937::Batch<>::create()) {}

    size_t nTrees;                          /*!< Number of trees in the forest. Default is 10 */
    double observationsPerTreeFraction;     /*!< Fraction of observations used for a training of one tree, 0 to 1.
                                                 Default is 1 (sampling with replacement) */
    size_t featuresPerNode;                 /*!< Number of features tried as possible splits per node.
                                                 If 0 then sqrt(p) for classification, p/3 for regression,
                                                 where p is the total number of features. */
    size_t maxTreeDepth;                    /*!< Maximal tree depth. Default is 0 (unlimited) */
    size_t minObservationsInLeafNode;       /*!< Minimal number of observations in a leaf node.
                                                 Default is 1 for classification, 5 for regression. */
    size_t seed;                            /*!< Seed for the random numbers generator used by the algorithms \DAAL_DEPRECATED_USE{ engine } */
    engines::EnginePtr engine;              /*!< Engine for the random numbers generator used by the algorithms */
    double impurityThreshold;               /*!< Threshold value used as stopping criteria: if the impurity value in the node is smaller
                                                 than the threshold then the node is not split anymore.*/
    VariableImportanceMode varImportance;   /*!< Variable importance computation mode */
    DAAL_UINT64 resultsToCompute;           /*!< 64 bit integer flag that indicates the results to compute */
    bool memorySavingMode;                  /*!< If true then use memory saving (but slower) mode */
    bool bootstrap;                         /*!< If true then training set for a tree is a bootstrap of the whole training set */
};
/* [Parameter source code] */
} // namespace interface1
using interface1::Parameter;
/** @} */
} // namespace training
}
}
} // namespace daal
#endif
