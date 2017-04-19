/* file: tanh_layer_forward_dense_default_batch_fpt_cpu.cpp */
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

//++
//  Implementation of hyperbolic tangent function calculation functions.
//--


#include "tanh_layer_forward_batch_container.h"
#include "tanh_layer_forward_kernel.h"
#include "tanh_layer_forward_impl.i"

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace tanh
{

namespace forward
{
namespace interface1
{
template class neural_networks::layers::tanh::forward::BatchContainer<DAAL_FPTYPE, defaultDense, DAAL_CPU>;
} // interface1
namespace internal
{
template class TanhKernel<DAAL_FPTYPE, defaultDense, DAAL_CPU>;
} // internal
} // forward

}
}
}
}
}
