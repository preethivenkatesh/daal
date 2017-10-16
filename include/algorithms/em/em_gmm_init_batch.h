/* file: em_gmm_init_batch.h */
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
//  Implementation of the interface of the EM for GMM initialization algorithm
//--
*/

#ifndef __EM_GMM_INIT_BATCH_H__
#define __EM_GMM_INIT_BATCH_H__

#include "algorithms/algorithm.h"
#include "data_management/data/numeric_table.h"
#include "services/daal_defines.h"
#include "em_gmm_init_types.h"

namespace daal
{
namespace algorithms
{
namespace em_gmm
{
namespace init
{

namespace interface1
{
/**
 * @defgroup em_gmm_init_batch Batch
 * @ingroup em_gmm_init
 * @{
 */
/**
 * <a name="DAAL-CLASS-ALGORITHMS__EM_GMM__INIT__BATCHCONTAINER"></a>
 * \brief Provides methods to compute initial values for the EM for GMM algorithm.
 *        The class is associated with the daal::algorithms::em_gmm::init::Batch class
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations of initial values for the EM for GMM algorithm, double or float
 */
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT BatchContainer : public daal::algorithms::AnalysisContainerIface<batch>
{
public:
     /**
     * Constructs a container for the EM for GMM initialization algorithm with a specified environment
     * in the batch processing mode
     * \param[in] daalEnv   Environment object
     */
    BatchContainer(daal::services::Environment::env *daalEnv);
    ~BatchContainer();
    /**
     * Computes initial values for the EM for GMM algorithm in the batch processing mode
     */
    virtual services::Status compute() DAAL_C11_OVERRIDE;
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__EM_GMM__INIT__BATCH"></a>
 * \brief Computes initial values for the EM for GMM algorithm in the batch processing mode.
 * <!-- \n<a href="DAAL-REF-EM_GMM-ALGORITHM">EM for GMM algorithm description and usage models</a> -->
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations of initial values for the EM for GMM algorithm, double or float
 *
 */

template<typename algorithmFPType = DAAL_ALGORITHM_FP_TYPE, Method method = defaultDense>
class DAAL_EXPORT Batch : public daal::algorithms::Analysis<batch>
{
public:
    Batch(const size_t nComponents) : parameter(nComponents)
    {
        initialize();
    }

    /**
     * Constructs an algorithm that computes initial values for the EM for GMM algorithm by copying input objects
     * and parameters of another algorithm that computes initial values for the EM for GMM algorithm
     * \param[in] other An algorithm to be used as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    Batch(const Batch<algorithmFPType, method> &other) : input(other.input), parameter(other.parameter)
    {
        initialize();
    }

    /**
    * Returns the method of the algorithm
    * \return Method of the algorithm
    */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int)0; }

    /**
     * Sets the memory for storing initial values for results of the EM for GMM algorithm
     * \param[in] result  Structure for storing initial values for results of the EM for GMM algorithm
     */
    services::Status setResult(const ResultPtr& result)
    {
        DAAL_CHECK(result, services::ErrorNullResult)
        _result = result;
        _res = _result.get();
        return services::Status();
    }

    /**
    * Returns the structure that contains initial values for the EM for GMM algorithm
    * \return Structure that contains initial values for the EM for GMM algorithm
    */
    ResultPtr getResult()
    {
        return _result;
    }

    /**
     * Returns a pointer to the newly allocated algorithm that computes initial values for the EM for GMM algorithm
     * with a copy of input objects of this algorithm
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<Batch<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Batch<algorithmFPType, method> >(cloneImpl());
    }

protected:
    virtual Batch<algorithmFPType, method> * cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Batch<algorithmFPType, method>(*this);
    }

    virtual services::Status allocateResult() DAAL_C11_OVERRIDE
    {
        services::Status s = _result->allocate<algorithmFPType>(&input, &parameter, 0);
        _res = _result.get();
        return s;
    }

    void initialize()
    {
        Analysis<batch>::_ac = new __DAAL_ALGORITHM_CONTAINER(batch, BatchContainer, algorithmFPType, method)(&_env);
        _in = &input;
        _par = &parameter;
        _result.reset(new Result());
    }

public:
    Input input;           /*!< %Input data structure */
    Parameter parameter;   /*!< %Parameter data structure */

private:
    ResultPtr _result;
};
/** @} */
} // namespace interface1
using interface1::BatchContainer;
using interface1::Batch;

}; // namespace init
} // namespace em_gmm
} // namespace algorithm
} // namespace daal
#endif
