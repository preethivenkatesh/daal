/* file: decision_tree_regression_training_types.h */
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

/*
//++
//  Implementation of the Decision tree algorithm interface
//--
*/

#ifndef __DECISION_TREE_REGRESSION_TRAINING_TYPES_H__
#define __DECISION_TREE_REGRESSION_TRAINING_TYPES_H__

#include "algorithms/algorithm.h"
#include "data_management/data/data_serialize.h"
#include "algorithms/decision_tree/decision_tree_regression_model.h"

namespace daal
{
namespace algorithms
{

/**
 * \brief Contains classes of the Decision tree algorithm
 */
namespace decision_tree
{

/**
 * \brief Contains classes of the Decision tree regression algorithm
 */
namespace regression
{

/**
 * @defgroup decision_tree_regression_training Training
 * \copydoc daal::algorithms::decision_tree::regression::training
 * @ingroup regression
 * @{
 */
/**
 * \brief Contains a class for Decision tree model-based training
 */
namespace training
{

/**
 * <a name="DAAL-ENUM-ALGORITHMS__DECISION_TREE__REGRESSION__TRAINING__METHOD"></a>
 * \brief Computation methods for Decision tree model-based training
 */
enum Method
{
    defaultDense = 0 /*!< Default method */
};

/**
 * <a name="DAAL-ENUM-ALGORITHMS__DECISION_TREE__REGRESSION__TRAINING__INPUTID"></a>
 * \brief Available identifiers of the results in the training stage of Decision tree
 */
enum InputId
{
    data                            = 0,        /*!< %Input data table */
    dependentVariables              = 1,        /*!< Values of the dependent variable for the input data */
    dataForPruning                  = 2,        /*!< Pruning data set */
    dependentVariablesForPruning    = 3         /*!< Labels of the pruning data set */
};

/**
 * <a name="DAAL-ENUM-ALGORITHMS__DECISION_TREE__TRAINING__RESULTID"></a>
 * \brief Available identifiers of the result of decision tree model-based training
 */
enum ResultId
{
    model = 0   /*!< Decision tree model */
};

/**
 * \brief Contains version 1.0 of the Intel(R) Data Analytics Acceleration Library (Intel(R) DAAL) interface
 */
namespace interface1
{

/**
 * <a name="DAAL-CLASS-ALGORITHMS__DECISION_TREE__REGRESSION__TRAINING__INPUT"></a>
 * \brief Base class for the input objects in the training stage of the regression algorithms
 */
class DAAL_EXPORT Input : public daal::algorithms::Input
{
public:
    Input();

    /**
     * Returns the input object in the training stage of the regression algorithm
     * \param[in] id   Identifier of the input object, \ref InputId
     * \return         Input object that corresponds to the given identifier
     */
    data_management::NumericTablePtr get(decision_tree::regression::training::InputId id) const;

    /**
     * Sets the input object in the training stage of the regression algorithm
     * \param[in] id    Identifier of the input object, \ref InputId
     * \param[in] value Pointer to the input object
     */
    void set(decision_tree::regression::training::InputId id, const data_management::NumericTablePtr & value);

    /**
     * Returns the number of columns in the input data set
     * \return Number of columns in the input data set
     */
    size_t getNumberOfFeatures() const;

    /**
    * Returns the number of dependent variables
    * \return Number of dependent variables
    */
    size_t getNumberOfDependentVariables() const;

    /**
     * Checks the correctness of the input object
     * \param[in] parameter Pointer to the structure of the algorithm parameters
     * \param[in] method    Computation method
     */
    services::Status check(const daal::algorithms::Parameter * parameter, int method) const DAAL_C11_OVERRIDE;

protected:
    services::Status checkImpl(const daal::algorithms::Parameter * parameter) const;
};

typedef services::SharedPtr<Input> InputPtr;
typedef services::SharedPtr<const Input> InputConstPtr;

/**
 * <a name="DAAL-CLASS-ALGORITHMS__DECISION_TREE__REGRESSION__TRAINING__RESULT"></a>
 * \brief Provides methods to access the result obtained with the compute() method of Decision tree model-based training
 */
class DAAL_EXPORT Result : public daal::algorithms::Result
{
public:
    DECLARE_SERIALIZABLE();
    Result();

    /**
     * Returns the result of Decision tree model-based training
     * \param[in] id    Identifier of the result
     * \return          Result that corresponds to the given identifier
     */
    ModelPtr get(ResultId id) const;

    /**
     * Allocates memory to store the result of Decision tree model-based training
     * \param[in] input Pointer to an object containing the input data
     * \param[in] parameter %Parameter of Decision tree model-based training
     * \param[in] method Computation method for the algorithm
     */
    template<typename algorithmFPType>
    DAAL_EXPORT services::Status allocate(const daal::algorithms::Input * input, const Parameter * parameter, int method);

protected:
    /** \private */
    template<typename Archive, bool onDeserialize>
    void serialImpl(Archive * arch) { daal::algorithms::Result::serialImpl<Archive, onDeserialize>(arch); }

    void serializeImpl(data_management::InputDataArchive * arch) DAAL_C11_OVERRIDE { serialImpl<data_management::InputDataArchive, false>(arch); }

    void deserializeImpl(data_management::OutputDataArchive * arch) DAAL_C11_OVERRIDE { serialImpl<data_management::OutputDataArchive, true>(arch); }
};

typedef services::SharedPtr<Result> ResultPtr;
typedef services::SharedPtr<const Result> ResultConstPtr;

} // namespace interface1

using interface1::Input;
using interface1::InputPtr;
using interface1::InputConstPtr;
using interface1::Result;
using interface1::ResultPtr;
using interface1::ResultConstPtr;

} // namespace training
/** @} */
} // namespace regression
} // namespace decision_tree
} // namespace algorithms
} // namespace daal

#endif
