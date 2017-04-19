/* file: prelu_layer_backward.cpp */
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
//  Implementation of prelu calculation algorithm and types methods.
//--
*/

#include "prelu_layer_backward_types.h"
#include "prelu_layer_types.h"
#include "serialization_utils.h"

using namespace daal::services;

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace prelu
{
namespace backward
{
namespace interface1
{
__DAAL_REGISTER_SERIALIZATION_CLASS(Result, SERIALIZATION_NEURAL_NETWORKS_LAYERS_PRELU_BACKWARD_RESULT_ID);
/** \brief Default constructor */
Input::Input() {};
Input::Input(const Input& other) : super(other) {}

/**
 * Returns an input object for the backward prelu layer
 * \param[in] id    Identifier of the input object
 * \return          %Input object that corresponds to the given identifier
 */
services::SharedPtr<data_management::Tensor> Input::get(LayerDataId id) const
{
    services::SharedPtr<layers::LayerData> layerData =
        services::staticPointerCast<layers::LayerData, data_management::SerializationIface>(Argument::get(layers::backward::inputFromForward));
    return services::staticPointerCast<data_management::Tensor, data_management::SerializationIface>((*layerData)[id]);
}

/**
 * Sets an input object for the backward prelu layer
 * \param[in] id     Identifier of the input object
 * \param[in] value  Pointer to the input object
 */
void Input::set(LayerDataId id, const services::SharedPtr<data_management::Tensor> &value)
{
    services::SharedPtr<layers::LayerData> layerData =
        services::staticPointerCast<layers::LayerData, data_management::SerializationIface>(Argument::get(layers::backward::inputFromForward));
    (*layerData)[id] = value;
}

/**
* Returns dimensions of auxWeights tensor
* \return Dimensions of auxWeights tensor
*/
services::Collection<size_t> Input::getAuxWeightsSizes(const layers::Parameter *par) const
{
    const Parameter *parameter =  static_cast<const Parameter *>(par);
    services::SharedPtr<data_management::Tensor> inputGradientTensor = get(layers::backward::inputGradient);

    size_t wStartDim = parameter->dataDimension;
    size_t wDimNumber = parameter->weightsDimension;

    services::Collection<size_t> _dims = inputGradientTensor->getDimensions();
    services::Collection<size_t> _wdims;

    for (size_t i = wStartDim; i < wStartDim + wDimNumber; i++)
    {
        _wdims.push_back(_dims[i]);
    }
    return _wdims;
}

/**
 * Checks an input object of the backward prelu layer
 * \param[in] par     Algorithm parameter
 * \param[in] method  Computation method
 */
services::Status Input::check(const daal::algorithms::Parameter *par, int method) const
{
    services::Status s;
    DAAL_CHECK_STATUS(s, layers::backward::Input::check(par, method));

    const Parameter *parameter = static_cast<const Parameter * >(par);
    const services::Collection<size_t> &inputDimensions = get(layers::backward::inputGradient)->getDimensions();
    DAAL_CHECK_EX(parameter->dataDimension <= inputDimensions.size() - parameter->weightsDimension, services::ErrorIncorrectParameter, services::ParameterName, dataDimensionStr());
    DAAL_CHECK_EX(parameter->weightsDimension != 0, services::ErrorIncorrectParameter, services::ParameterName, weightsDimensionStr());

    const services::Collection<size_t> weightsDimensions = getAuxWeightsSizes(parameter);

    DAAL_CHECK_STATUS(s, data_management::checkTensor(get(prelu::auxData).get(), auxDataStr(), &inputDimensions));
    DAAL_CHECK_STATUS(s, data_management::checkTensor(get(prelu::auxWeights).get(), auxWeightsStr(), &weightsDimensions));

    return s;
}

/** \brief Default constructor */
Result::Result() : layers::backward::Result() {};

/**
 * Checks the result of the backward prelu layer
 * \param[in] input   %Input object for the algorithm
 * \param[in] par     %Parameter of the algorithm
 * \param[in] method  Computation method
 */
services::Status Result::check(const daal::algorithms::Input *input, const daal::algorithms::Parameter *par, int method) const
{
    const Input *in = static_cast<const Input *>(input);
    const Parameter *param = static_cast<const Parameter *>(par);

    services::Status s;
    if (param->propagateGradient)
    {
        DAAL_CHECK_STATUS(s, data_management::checkTensor(get(layers::backward::gradient).get(), gradientStr(),
                                                          &(in->get(prelu::auxData)->getDimensions())));
    }
    DAAL_CHECK_STATUS(s, data_management::checkTensor(get(layers::backward::weightDerivatives).get(), weightDerivativesStr(),
                                                      &(in->get(prelu::auxWeights)->getDimensions())));
    return s;
}

}// namespace interface1
}// namespace backward
}// namespace prelu
}// namespace layers
}// namespace neural_networks
}// namespace algorithms
}// namespace daal
