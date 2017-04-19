/* file: dropout_layer_backward_impl.i */
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
//  Implementation of the backward dropout layer
//--
*/

#ifndef __DROPOUT_LAYER_BACKWARD_IMPL_I__
#define __DROPOUT_LAYER_BACKWARD_IMPL_I__

using namespace daal::internal;
using namespace daal::services;

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
namespace dropout
{
namespace backward
{
namespace internal
{

template<typename algorithmFPType, Method method, CpuType cpu>
services::Status DropoutKernel<algorithmFPType, method, cpu>::compute(Tensor *inputGradientTable, Tensor *maskTable, Tensor *resultTable, const dropout::Parameter *parameter)
{
    const services::Collection<size_t> &dims = inputGradientTable->getDimensions();
    size_t nInputRows = dims[0];

    size_t nBlocks = nInputRows / _nRowsInBlock;
    size_t nRowsInLastBlock = nInputRows - nBlocks * _nRowsInBlock;

    services::Status s;
    for(size_t block = 0; block < nBlocks; block++)
    {
        s |= processBlock(inputGradientTable, maskTable, block * _nRowsInBlock, _nRowsInBlock, resultTable);
    }
    if(nRowsInLastBlock > 0)
    {
        s |= processBlock(inputGradientTable, maskTable, nBlocks * _nRowsInBlock, nRowsInLastBlock, resultTable);
    }
    return s;
}

template<typename algorithmFPType, Method method, CpuType cpu>
inline services::Status DropoutKernel<algorithmFPType, method, cpu>::processBlock(Tensor *inputGradientTable,
        Tensor *maskTable, size_t nProcessedRows, size_t nRowsInCurrentBlock, Tensor *resultTable)
{
    SubtensorDescriptor<algorithmFPType> inputGradientBlock;
    inputGradientTable->getSubtensor(0, 0, nProcessedRows, nRowsInCurrentBlock, readOnly, inputGradientBlock);
    algorithmFPType *inputGradientArray = inputGradientBlock.getPtr();

    SubtensorDescriptor<algorithmFPType> maskBlock;
    maskTable->getSubtensor(0, 0, nProcessedRows, nRowsInCurrentBlock, readOnly, maskBlock);
    algorithmFPType *maskArray = maskBlock.getPtr();

    SubtensorDescriptor<algorithmFPType> resultBlock;
    resultTable->getSubtensor(0, 0, nProcessedRows, nRowsInCurrentBlock, writeOnly, resultBlock);
    algorithmFPType *resultArray = resultBlock.getPtr();

    size_t nDataElements = inputGradientBlock.getSize();
    for(size_t i = 0; i < nDataElements; i++)
    {
        resultArray[i] = inputGradientArray[i] * maskArray[i];
    }

    inputGradientTable->releaseSubtensor(inputGradientBlock);
    maskTable->releaseSubtensor(maskBlock);
    resultTable->releaseSubtensor(resultBlock);
    return services::Status();
}

} // internal
} // backward
} // namespace dropout
} // namespace layers
} // namespace neural_networks
} // namespace algorithms
} // namespace daal

#endif
