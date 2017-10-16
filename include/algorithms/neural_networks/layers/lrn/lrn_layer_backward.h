/* file: lrn_layer_backward.h */
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
//  Implementation of the interface for the backward local response normalization layer
//  in the batch processing mode
//--
*/

#ifndef __LRN_LAYER_BACKWARD_H__
#define __LRN_LAYER_BACKWARD_H__

#include "algorithms/algorithm.h"
#include "data_management/data/tensor.h"
#include "services/daal_defines.h"
#include "algorithms/neural_networks/layers/layer.h"
#include "algorithms/neural_networks/layers/lrn/lrn_layer_types.h"
#include "algorithms/neural_networks/layers/lrn/lrn_layer_backward_types.h"

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace lrn
{
namespace backward
{
namespace interface1
{
/**
 * @defgroup lrn_backward_batch Batch
 * @ingroup lrn_backward
 * @{
 */
/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__LAYERS__LRN__BACKWARD__BATCHCONTAINER"></a>
* \brief Provides methods to run implementations of the of the backward local response normalization layer
*        This class is associated with the daal::algorithms::neural_networks::layers::lrn::backward::Batch class
*        and supports the method of backward local response normalization layer computation in the batch processing mode
*
* \tparam algorithmFPType  Data type to use in intermediate computations of backward local response normalization layer, double or float
* \tparam method           Computation method of the layer, \ref daal::algorithms::neural_networks::layers::lrn::Method
* \tparam cpu              Version of the cpu-specific implementation of the layer, \ref daal::CpuType
*/
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT BatchContainer : public AnalysisContainerIface<batch>
{
public:
    /**
     * Constructs a container for the backward local response normalization layer with a specified environment
     * in the batch processing mode
     * \param[in] daalEnv   Environment object
     */
    BatchContainer(daal::services::Environment::env *daalEnv);
    /** Default destructor */
    ~BatchContainer();
    /**
     * Computes the result of the backward local response normalization layer in the batch processing mode
     *
     * \return Status of computations
     */
    services::Status compute() DAAL_C11_OVERRIDE;
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__LAYERS__LRN__BACKWARD__BATCH"></a>
 * \brief Provides methods for the backward local response normalization layer in the batch processing mode
 * <!-- \n<a href="DAAL-REF-LRNBACKWARD-ALGORITHM">Backward local response normalization layer description and usage models</a> -->
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations for the backward local response normalization layer, double or float
 * \tparam method           Backward local response normalization layer method, \ref Method
 *
 * \par Enumerations
 *      - \ref Method                      Computation methods for the backward local response normalization layer
 *      - \ref backward::InputId           Identifiers of input objects for the backward local response normalization layer
 *      - \ref LayerDataId                 Identifiers of collection in input objects for the backward local response normalization layer
 *      - \ref backward::InputLayerDataId  Identifiers of extra results computed by the forward local response normalization layer
 *      - \ref backward::ResultId          Identifiers of result objects for the backward local response normalization layer
 *
 * \par References
 *      - \ref forward::interface1::Batch "forward::Batch" class
 */
template<typename algorithmFPType = DAAL_ALGORITHM_FP_TYPE, Method method = defaultDense>
class Batch : public layers::backward::LayerIfaceImpl
{
public:
    typedef layers::backward::LayerIfaceImpl super;
    Parameter &parameter; /*!< Backward local response normalization layer \ref interface1::Parameter "parameters" */
    Input input;          /*!< Backward local response normalization layer input */

    /** Default constructor */
    Batch() : parameter(_defaultParameter)
    {
        initialize();
    };

    /**
     * Constructs a backward local response normalization layer in the batch processing mode
     * and initializes its parameter with the provided parameter
     * \param[in] parameter Parameter to initialize the parameter of the layer
     */
    Batch(Parameter& parameter) : parameter(parameter), _defaultParameter(parameter)
    {
        initialize();
    }

    /**
     * Constructs a backward local response normalization layer by copying input objects
     * and parameters of another backward local response normalization layer in the batch processing mode
     * \param[in] other Algorithm to use as the source to initialize the input objects
     *                  and parameters of the layer
     */
    Batch(const Batch<algorithmFPType, method> &other) : super(other),
        _defaultParameter(other.parameter), parameter(_defaultParameter), input(other.input)
    {
        initialize();
    }

    /**
    * Returns the method of the layer
    * \return Method of the layer
    */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int) method; }

    /**
     * Returns the structure that contains the input objects of backward local response normalization layer
     * \return Structure that contains the input objects of backward local response normalization layer
     */
    virtual Input *getLayerInput() DAAL_C11_OVERRIDE { return &input; }

    /**
     * Returns the structure that contains parameters of the backward local response normalization layer
     * \return Structure that contains parameters of the backward local response normalization layer
     */
    virtual Parameter *getLayerParameter() DAAL_C11_OVERRIDE { return &parameter; };

    /**
     * Returns the structure that contains result of the backward local response normalization layer
     * \return Structure that contains result of the backward local response normalization layer
     */
    layers::backward::ResultPtr getLayerResult() DAAL_C11_OVERRIDE
    {
        return _result;
    }

    /**
     * Returns the structure that contains the result of the backward local response normalization layer
     * \return Structure that contains the result of the backward local response normalization layer
     */
    ResultPtr getResult()
    {
        return _result;
    }

    /**
     * Registers user-allocated memory to store the result of the backward local response normalization layer
     * \param[in] result Structure to store the result of the backward local response normalization layer
     *
     * \return Status of computations
     */
    services::Status setResult(const ResultPtr& result)
    {
        DAAL_CHECK(result, services::ErrorNullResult)
        _result = result;
        _res = _result.get();
        return services::Status();
    }

    /**
     * Returns a pointer to a newly allocated backward local response normalization layer
     * with a copy of the input objects and parameters for this backward local response normalization layer
     * in the batch processing mode
     * \return Pointer to the newly allocated layer
     */
    services::SharedPtr<Batch<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Batch<algorithmFPType, method> >(cloneImpl());
    }

    /**
    * Allocates memory to store the result of the backward local response normalization layer
    *
     * \return Status of computations
    */
    virtual services::Status allocateResult() DAAL_C11_OVERRIDE
    {
        services::Status s = this->_result->template allocate<algorithmFPType>(&(this->input), &parameter, (int) method);
        this->_res = this->_result.get();
        return s;
    }

protected:
    virtual Batch<algorithmFPType, method> *cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Batch<algorithmFPType, method>(*this);
    }

    void initialize()
    {
        Analysis<batch>::_ac = new __DAAL_ALGORITHM_CONTAINER(batch, BatchContainer, algorithmFPType, method)(&_env);
        _in = &input;
        _par = &parameter;
        _result.reset(new Result());
    }

private:
    ResultPtr _result;
    Parameter _defaultParameter;
};
/** @} */
} // namespace interface1
using interface1::BatchContainer;
using interface1::Batch;
} // namespace backward
} // namespace lrn
} // namespace layers
} // namespace neural_networks
} // namespace algorithms
} // namespace daal
#endif
