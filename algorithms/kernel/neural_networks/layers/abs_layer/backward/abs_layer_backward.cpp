/* file: abs_layer_backward.cpp */
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
//  Implementation of abs calculation algorithm and types methods.
//--
*/

#include "abs_layer_backward_types.h"
#include "abs_layer_types.h"
#include "serialization_utils.h"
#include "daal_strings.h"

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace abs
{
namespace backward
{
namespace interface1
{
__DAAL_REGISTER_SERIALIZATION_CLASS(Result, SERIALIZATION_NEURAL_NETWORKS_LAYERS_ABS_BACKWARD_RESULT_ID);
/** Default constructor */
Input::Input() {};
Input::Input(const Input& other) : super(other) {}

/**
* Returns an input object for the backward abs layer
* \param[in] id    Identifier of the input object
* \return          %Input object that corresponds to the given identifier
*/
data_management::TensorPtr Input::get(LayerDataId id) const
{
    layers::LayerDataPtr layerData =
        services::staticPointerCast<layers::LayerData, data_management::SerializationIface>(Argument::get(layers::backward::inputFromForward));
    return services::staticPointerCast<data_management::Tensor, data_management::SerializationIface>((*layerData)[id]);
}

/**
 * Sets an input object for the backward abs layer
 * \param[in] id      Identifier of the input object
 * \param[in] value   Pointer to the object
 */
void Input::set(LayerDataId id, const data_management::TensorPtr &value)
{
    layers::LayerDataPtr layerData =
        services::staticPointerCast<layers::LayerData, data_management::SerializationIface>(Argument::get(layers::backward::inputFromForward));
    (*layerData)[id] = value;
}

/**
* Checks input object for the backward abs layer
* \param[in] par     Algorithm parameter
* \param[in] method  Computation method
*/
services::Status Input::check(const daal::algorithms::Parameter *par, int method) const
{
    const layers::Parameter *parameter = static_cast<const layers::Parameter * >(par);
    if (!parameter->propagateGradient) { return services::Status(); }

    services::Status s = layers::backward::Input::check(par, method);
    if(!s) { return s; }

    const services::Collection<size_t>& inputDimensions = get(layers::backward::inputGradient)->getDimensions();

    if(!parameter->predictionStage)
    {
        s |= data_management::checkTensor(get(auxData).get(), auxDataStr(), &inputDimensions);
    }
    return s;
}

Result::Result() : layers::backward::Result() {};

/**
 * Checks the result of the backward abs layer
 * \param[in] input   %Input object for the algorithm
 * \param[in] par     %Parameter of the algorithm
 * \param[in] method  Computation method
 */
services::Status Result::check(const daal::algorithms::Input *input, const daal::algorithms::Parameter *par, int method) const
{
    const layers::Parameter *parameter = static_cast<const layers::Parameter * >(par);
    if (!parameter->propagateGradient) { return services::Status(); }

    const Input *in = static_cast<const Input *>(input);
    return data_management::checkTensor(get(layers::backward::gradient).get(), gradientStr(), &(in->get(auxData)->getDimensions()));
}

}// namespace interface1
}// namespace backward
}// namespace abs
}// namespace layers
}// namespace neural_networks
}// namespace algorithms
}// namespace daal
