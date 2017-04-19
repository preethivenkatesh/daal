/* file: kernel_function_dense_base.h */
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
//  Declaration of template structs that calculate SVM Kernel functions.
//--
*/

#ifndef __KERNEL_FUNCTION_DENSE_BASE_H__
#define __KERNEL_FUNCTION_DENSE_BASE_H__

#include "numeric_table.h"
#include "kernel_function_types_linear.h"
#include "kernel_function_types_rbf.h"
#include "kernel_function_linear.h"
#include "kernel_function_rbf.h"
#include "service_micro_table.h"
#include "kernel.h"

using namespace daal::internal;

namespace daal
{
namespace algorithms
{
namespace kernel_function
{
namespace internal
{

template <typename algorithmFPType, CpuType cpu>
struct KernelImplBase : public Kernel
{
    void prepareDataVectorVector(
        BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA1,
        BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA2,
        BlockMicroTable<algorithmFPType, writeOnly, cpu> &mtR,
        const ParameterBase *svmPar,
        size_t *nVectors1, algorithmFPType **dataA1, size_t *nVectors2, algorithmFPType **dataA2,
        algorithmFPType **dataR, bool inputTablesSame)
    {
        mtA1.getBlockOfRows(svmPar->rowIndexX, 1, dataA1);
        mtA2.getBlockOfRows(svmPar->rowIndexY, 1, dataA2);
        mtR. getBlockOfRows(svmPar->rowIndexResult, 1, dataR);
    }

    void prepareDataMatrixVector(
        BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA1,
        BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA2,
        BlockMicroTable<algorithmFPType, writeOnly, cpu> &mtR,
        const ParameterBase *svmPar,
        size_t *nVectors1, algorithmFPType **dataA1, size_t *nVectors2, algorithmFPType **dataA2,
        algorithmFPType **dataR, bool inputTablesSame)
    {
        size_t n = mtA1.getFullNumberOfRows();
        mtA1.getBlockOfRows(0, n, dataA1);
        mtA2.getBlockOfRows(svmPar->rowIndexY, 1, dataA2);
        mtR. getBlockOfRows(svmPar->rowIndexResult, 1, dataR);
        *nVectors1 = n;
    }

    void prepareDataMatrixMatrix(
        BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA1,
        BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA2,
        BlockMicroTable<algorithmFPType, writeOnly, cpu> &mtR,
        const ParameterBase *svmPar,
        size_t *nVectors1, algorithmFPType **dataA1, size_t *nVectors2, algorithmFPType **dataA2,
        algorithmFPType **dataR, bool inputTablesSame)
    {
        size_t n1 = mtA1.getFullNumberOfRows();
        size_t n2 = mtA2.getFullNumberOfRows();
        mtA1.getBlockOfRows(0, n1, dataA1);
        if (!inputTablesSame) { mtA2.getBlockOfRows(0, n2, dataA2); }
        mtR. getBlockOfRows(0, n1, dataR);
        *nVectors1 = n1;
        *nVectors2 = n2;
    }

    virtual void computeInternal(size_t nFeatures, size_t nVectors1, const algorithmFPType *dataA1,
                                 size_t nVectors2, const algorithmFPType *dataA2,
                                 algorithmFPType *dataR, const ParameterBase *par, bool inputTablesSame) = 0;

    virtual void prepareData(BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA1,
                             BlockMicroTable<algorithmFPType, readOnly,  cpu> &mtA2,
                             BlockMicroTable<algorithmFPType, writeOnly, cpu> &mtR,
                             const ParameterBase *svmPar,
                             size_t *nVectors1, algorithmFPType **dataA1,  size_t *nVectors2, algorithmFPType **dataA2,
                             algorithmFPType **dataR, bool inputTablesSame) = 0;

    services::Status compute(ComputationMode computationMode, const NumericTable *a1, const NumericTable *a2, NumericTable *r,
                 const daal::algorithms::Parameter *par)
    {

        _computationMode = computationMode;

        BlockMicroTable<algorithmFPType, readOnly,  cpu> mtA1(a1);
        BlockMicroTable<algorithmFPType, readOnly,  cpu> mtA2(a2);
        BlockMicroTable<algorithmFPType, writeOnly, cpu> mtR(r);
        size_t nFeatures = mtA1.getFullNumberOfColumns();
        size_t nVectors1, nVectors2;
        algorithmFPType *dataA1, *dataA2, *dataR;
        const ParameterBase *svmPar = static_cast<const ParameterBase *>(par);

        bool inputTablesSame = ((a1 == a2) ? true : false);

        prepareData(mtA1, mtA2, mtR, svmPar, &nVectors1, &dataA1, &nVectors2, &dataA2, &dataR, inputTablesSame);
        computeInternal(nFeatures, nVectors1, dataA1, nVectors2, dataA2, dataR, svmPar, inputTablesSame);

        mtA1.release();
        mtA2.release();
        mtR. release();
        DAAL_RETURN_STATUS()
    }

protected:
    ComputationMode _computationMode;
};

} // namespace internal

} // namespace kernel_function

} // namespace algorithms

} // namespace daal

#endif
