/* file: pca_partialresult_correlation.h */
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
//  Implementation of PCA algorithm interface.
//--
*/

#ifndef __PCA_PARTIALRESULT_CORRELATION_
#define __PCA_PARTIALRESULT_CORRELATION_

#include "algorithms/pca/pca_types.h"

namespace daal
{
namespace algorithms
{
namespace pca
{

/**
 * Allocates memory to store partial results of the PCA  SVD algorithm
 * \param[in] input     Pointer to an object containing input data
 * \param[in] parameter Pointer to the structure of algorithm parameters
 * \param[in] method    Computation method
 */
template<typename algorithmFPType>
DAAL_EXPORT services::Status PartialResultImpl<algorithmFPType, correlationDense>::allocate(const daal::algorithms::Input *input, const daal::algorithms::Parameter *parameter, const int method)
{
    set(nObservationsCorrelation,
        data_management::NumericTablePtr(
            new data_management::HomogenNumericTable<algorithmFPType>(1, 1, data_management::NumericTableIface::doAllocate, 0)));
    set(sumCorrelation,
        data_management::NumericTablePtr(
            new data_management::HomogenNumericTable<algorithmFPType>((static_cast<const InputIface *>(input))->getNFeatures(), 1,
                                                             data_management::NumericTableIface::doAllocate, 0)));
    set(crossProductCorrelation,
        data_management::NumericTablePtr(
            new data_management::HomogenNumericTable<algorithmFPType>((static_cast<const InputIface *>(input))->getNFeatures(),
                                                             (static_cast<const InputIface *>(input))->getNFeatures(),
                                                             data_management::NumericTableIface::doAllocate, 0)));
    return services::Status();
};

} // namespace pca
} // namespace algorithms
} // namespace daal

#endif
