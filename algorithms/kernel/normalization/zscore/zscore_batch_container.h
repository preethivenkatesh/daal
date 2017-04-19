/* file: zscore_batch_container.h */
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
//  Implementation of zscore normalization calculation algorithm container.
//--
*/

#include "normalization/zscore.h"
#include "zscore_base.h"
#include "zscore_dense_default_kernel.h"
#include "zscore_dense_sum_kernel.h"

namespace daal
{
namespace algorithms
{
namespace normalization
{
namespace zscore
{
namespace interface1
{

template<typename algorithmFPType, Method method, CpuType cpu>
BatchContainer<algorithmFPType, method, cpu>::BatchContainer(daal::services::Environment::env *daalEnv) : AnalysisContainerIface<batch>(daalEnv)
{
    __DAAL_INITIALIZE_KERNELS(internal::ZScoreKernel, algorithmFPType, method);
}

template<typename algorithmFPType, Method method, CpuType cpu>
BatchContainer<algorithmFPType, method, cpu>::~BatchContainer()
{
    __DAAL_DEINITIALIZE_KERNELS();
}

template<typename algorithmFPType, Method method, CpuType cpu>
services::Status BatchContainer<algorithmFPType, method, cpu>::compute()
{
    Input *input = static_cast<Input *>(_in);
    Result *result = static_cast<Result *>(_res);
    daal::algorithms::Parameter *parameter = _par;
    daal::services::Environment::env &env = *_env;

    SharedPtr<NumericTable> inputTable  = input->get(data);
    NumericTable *resultTable = result->get(normalizedData).get();
    NumericTable *sumTable    = inputTable->basicStatistics.get(NumericTableIface::sum).get();

    __DAAL_CALL_KERNEL(env, internal::ZScoreKernel, __DAAL_KERNEL_ARGUMENTS(algorithmFPType, method), compute, inputTable, sumTable, resultTable, parameter);

}
}
} // namespace zscore
} // namespace normalization
} // namespace algorithms
} // namespace daal
