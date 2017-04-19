/* file: decision_forest_regression_training_batch.h */
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
//  Implementation of the interface for decision forest model-based training
//  in the batch processing mode
//--
*/

#ifndef __DECISION_FOREST_REGRESSSION_TRAINING_BATCH_H__
#define __DECISION_FOREST_REGRESSSION_TRAINING_BATCH_H__

#include "algorithms/algorithm.h"
#include "data_management/data/numeric_table.h"
#include "services/daal_defines.h"
#include "services/daal_memory.h"
#include "algorithms/decision_forest/decision_forest_regression_training_types.h"
#include "algorithms/decision_forest/decision_forest_regression_model.h"

namespace daal
{
namespace algorithms
{
namespace decision_forest
{
namespace regression
{
namespace training
{
namespace interface1
{
/**
 * @defgroup decision_forest_regression_batch Batch
 * @ingroup decision_forest_regression_training
 * @{
 */
/**
 * <a name="DAAL-CLASS-ALGORITHMS__DECISION_FOREST__REGRESSION__TRAINING__BATCHCONTAINER"></a>
 * \brief Class containing methods for decision forest regression
 *        model-based training using algorithmFPType precision arithmetic
 */
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT BatchContainer : public TrainingContainerIface<batch>
{
public:
    /**
     * Constructs a container for decision forest model-based training with a specified environment
     * in the batch processing mode
     * \param[in] daalEnv   Environment object
     */
    BatchContainer(daal::services::Environment::env *daalEnv);
    /** Default destructor */
    ~BatchContainer();
    /**
     * Computes the result of decision forest model-based training in the batch processing mode
     * \return Status of computations
     */
    services::Status compute() DAAL_C11_OVERRIDE;
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__DECISION_FOREST__REGRESSION__TRAINING__BATCH"></a>
 * \brief Provides methods for decision forest model-based training in the batch processing mode
 * <!-- \n<a href="DAAL-REF-DECISION_FOREST__REGRESSION__TRAINING-ALGORITHM">decision forest algorithm description and usage models</a> -->
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations for decision forest model-based training, double or float
 * \tparam method           decision forest training method, \ref Method
 *
 * \par Enumerations
 *      - \ref Method  Computation methods
 *
 * \par References
 *      - \ref decision_forest::regression::interface1::Model "Model" class
 *      - \ref prediction::interface1::Batch "prediction::Batch" class
 */
template<typename algorithmFPType = double, Method method = defaultDense>
class DAAL_EXPORT Batch : public Training<batch>
{
public:
    Input input; /*!< %Input data structure */
    Parameter parameter; /*!< %Training \ref interface1::Parameter "parameters" */

    /** Default constructor */
    Batch() : parameter()
    {
        initialize();
        parameter.minObservationsInLeafNode = 5;
    }

    /**
     * Constructs a decision forest training algorithm by copying input objects
     * and parameters of another decision forest training algorithm in the batch processing mode
     * \param[in] other Algorithm to use as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    Batch(const Batch<algorithmFPType, method> &other) : input(other.input), parameter(other.parameter)
    {
    }

    ~Batch() {}

    /**
    * Returns the method of the algorithm
    * \return Method of the algorithm
    */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int)method; }

    /**
     * Registers user-allocated memory to store the result of decision forest model-based training
     * \param[in] res    Structure to store the result of decision forest model-based training
     * \return Status
     */
    services::Status setResult(const services::SharedPtr<Result>& res)
    {
        DAAL_CHECK(res,  services::ErrorNullResult)
        _result = res;
        _res = _result.get();
        return services::Status();
    }

    /**
     * Returns the structure that contains the result of decision forest model-based training
     * \return Structure that contains the result of decision forest model-based training
     */
    services::SharedPtr<Result> getResult() { return _result; }

    /**
     * Returns a pointer to a newly allocated decision forest training algorithm
     * with a copy of the input objects and parameters for this decision forest training algorithm
     * in the batch processing mode
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<Batch<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Batch<algorithmFPType, method> >(cloneImpl());
    }

protected:
    services::SharedPtr<Result> _result;

    virtual Batch<algorithmFPType, method> * cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Batch<algorithmFPType, method>(*this);
    }

    services::Status allocateResult() DAAL_C11_OVERRIDE
    {
        services::Status s = _result->allocate<algorithmFPType>(&input, &parameter, method);
        _res = _result.get();
        return s;
    }

    void initialize()
    {
        _ac  = new __DAAL_ALGORITHM_CONTAINER(batch, BatchContainer, algorithmFPType, method)(&_env);
        _in = &input;
        _par = &parameter;
        _result = services::SharedPtr<Result>(new Result());
    }
};
/** @} */
} // namespace interface1
using interface1::BatchContainer;
using interface1::Batch;
}
}
}
}
}
#endif
