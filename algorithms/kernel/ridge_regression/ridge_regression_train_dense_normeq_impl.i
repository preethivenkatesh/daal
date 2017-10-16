/* file: ridge_regression_train_dense_normeq_impl.i */
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
//  Implementation of auxiliary functions for ridge regression Normal Equations (normEqDense) method.
//--
*/

#ifndef __RIDGE_REGRESSION_TRAIN_DENSE_NORMEQ_IMPL_I__
#define __RIDGE_REGRESSION_TRAIN_DENSE_NORMEQ_IMPL_I__

#include "service_memory.h"
#include "service_blas.h"
#include "service_lapack.h"
#include "ridge_regression_ne_model.h"
#include "ridge_regression_train_kernel.h"
#include "threading.h"
#include "daal_defines.h"
#include "service_memory.h"

namespace daal
{
namespace algorithms
{
namespace ridge_regression
{

using namespace daal::services::internal;
using namespace daal::internal;

#define DEFINE_TABLE_BLOCK_EX(BlockType, targetVariable, ...)    \
    BlockType<algorithmFPType, cpu> targetVariable(__VA_ARGS__); \
    DAAL_CHECK_BLOCK_STATUS(targetVariable);

#define DEFINE_TABLE_BLOCK(BlockType, targetVariable, table) \
    DEFINE_TABLE_BLOCK_EX(BlockType, targetVariable, table, 0, table->getNumberOfRows())

namespace training
{
namespace internal
{

template <typename algorithmFPType, CpuType cpu>
static void updatePartialSums(DAAL_INT *p,                  /* features */
                              DAAL_INT *n,                  /* vectors */
                              DAAL_INT *b,                  /* features + 1 */
                              algorithmFPType  *x_in,       /* p*n input matrix */
                              algorithmFPType  *xtx_out,    /* p*b output matrix */
                              DAAL_INT *v,                  /* variables */
                              algorithmFPType  *y_in,       /* v*n input matrix   */
                              algorithmFPType  *xty_out     /* v*b output matrix */ )
{
    size_t i, j;

    DAAL_INT p_val = *p;
    DAAL_INT n_val = *n;
    DAAL_INT b_val = *b;
    DAAL_INT v_val = *v;

    char uplo   = 'U';
    char trans  = 'N';
    char transa = 'N';
    char transb = 'T';

    algorithmFPType alpha = 1.0;
    algorithmFPType* xtx_ptr;
    algorithmFPType* x_ptr;
    algorithmFPType* y_ptr;

    Blas<algorithmFPType, cpu>::xxsyrk(&uplo, &trans, p, n, &alpha, x_in, p, &alpha, xtx_out, b);

    if ( p_val < b_val )
    {
        xtx_ptr = xtx_out + p_val * b_val;

      PRAGMA_IVDEP
      PRAGMA_VECTOR_ALWAYS
        for ( i = 0, x_ptr = x_in; i < n_val; i++, x_ptr += p_val)
        {
            for ( j = 0; j < p_val; j++)
            {
                xtx_ptr[j] += x_ptr[j];
            }
        }

        xtx_ptr[p_val] += (algorithmFPType)n_val;

    } /* if ( p_val < b_val ) */

    Blas<algorithmFPType, cpu>::xxgemm(&transa, &transb, p, v, n, &alpha, x_in, p, y_in, v, &alpha, xty_out, b);

    if ( p_val < b_val )
    {
        for ( i = 0, y_ptr = y_in; i < n_val; i++, y_ptr += v_val)
        {
          PRAGMA_IVDEP
          PRAGMA_VECTOR_ALWAYS
            for (j = 0; j < v_val; j++)
            {
                xty_out[j * b_val + p_val] += y_ptr[j];
            }
        }

    } /* if ( p_val < b_val ) */

} // updatePartialSums

/**
 *  \brief Function that calculates ridge regression coefficients
 *         from matrices X'*X and X'*Y.
 *
 *  \param p[in]               Number of rows in input matrix X'*X
 *  \param xtx[in]             Input matrix X'*X
 *  \param ldxtx[in]           Leading dimension of matrix X'*X (ldxtx >= p)
 *  \param ny[in]              Number of rows in input matrix X'*Y
 *  \param xty[in]             Input matrix X'*Y
 *  \param ldxty[in]           Leading dimension of matrix X'*Y (ldxty >= p)
 *  \param isEqualRidge[in]    Flag that indicates if equal ridge parameters must be used
 *  \param ridge[in]           Ridge parameters of size 1 x 1 or 1 x ny (depends on isEqualRidge)
 *  \param beta[out]           Resulting matrix of coefficients of size ny x ldxty
 */
template <typename algorithmFPType, CpuType cpu>
Status computeRidgeCoeffs(DAAL_INT *p,  algorithmFPType *xtx, DAAL_INT *ldxtx,
                          DAAL_INT *ny, algorithmFPType *xty, DAAL_INT *ldxty,
                          bool isEqualRidge, bool interceptFlag,
                          algorithmFPType *ridge, algorithmFPType *beta)
{
    DAAL_INT n;
    DAAL_INT i_one = 1;
    char uplo = 'U';
    DAAL_INT info;

    n = (*ny) * (*ldxty);
    daal::services::daal_memcpy_s(beta, n * sizeof(algorithmFPType), xty, n * sizeof(algorithmFPType));

    // If interceptFlag == false, all diagonal elements of xtx to be fixed. Otherwise, all diagonal elements of xtx except a last one, which
    // corresponds to zeroth beta.
    const size_t pToFix = (interceptFlag ? static_cast<size_t>(*p) - 1 : static_cast<size_t>(*p));

    const size_t xtxSize = (*p) * (*ldxtx);
    const size_t xtxSizeInBytes = xtxSize * sizeof(algorithmFPType);

    TArray<algorithmFPType, cpu> tempXTXArray(xtxSize);
    algorithmFPType *tempXTX = tempXTXArray.get();
    DAAL_CHECK_MALLOC(tempXTX);

    if (isEqualRidge)
    {
        daal::services::daal_memcpy_s(tempXTX, xtxSizeInBytes, xtx, xtxSizeInBytes);

        // X' * X <- X' * X + ridge * I
        algorithmFPType * ptr = tempXTX;
        for (size_t i = 0; i < pToFix; ++i)
        {
            *ptr += *ridge;
            ptr += *p + 1;
        }

        /* Perform L*L' decomposition of X'*X */
        Lapack<algorithmFPType, cpu>::xpotrf(&uplo, p, tempXTX, ldxtx, &info);
        if (info < 0) { return Status(services::ErrorRidgeRegressionInternal);                   }
        if (info > 0) { return Status(services::ErrorRidgeRegressionNormEqSystemSolutionFailed); }

        /* Solve L*L'*b=Y */
        Lapack<algorithmFPType, cpu>::xpotrs(&uplo, p, ny, tempXTX, ldxtx, beta, ldxty, &info);
        DAAL_CHECK(info == 0, services::ErrorRidgeRegressionInternal);
    }
    else
    {
        algorithmFPType * betaPtr = beta;
        for (size_t j = 0; j < static_cast<size_t>(*ny); ++j)
        {
            daal::services::daal_memcpy_s(tempXTX, xtxSizeInBytes, xtx, xtxSizeInBytes);
            algorithmFPType * ptr = tempXTX;
            for (size_t i = 0; i < pToFix; ++i)
            {
                *ptr += ridge[j];
                ptr += *p + 1;
            }

            /* Perform L*L' decomposition of X'*X */
            Lapack<algorithmFPType, cpu>::xpotrf(&uplo, p, tempXTX, ldxtx, &info);
            if (info < 0) { return Status(services::ErrorRidgeRegressionInternal);                   }
            if (info > 0) { return Status(services::ErrorRidgeRegressionNormEqSystemSolutionFailed); }

            /* Solve L*L'*b=Y */
            Lapack<algorithmFPType, cpu>::xpotrs(&uplo, p, &i_one, tempXTX, ldxtx, betaPtr, ldxty, &info);
            DAAL_CHECK(info == 0, services::ErrorRidgeRegressionInternal);

            betaPtr += *ldxty;
        }
    }

    return Status();
}


template <typename algorithmFPType, CpuType cpu>
Status updatePartialModelNormEq(NumericTable *x,
                                NumericTable *y,
                                ridge_regression::Model *r,
                                const daal::algorithms::Parameter *par,
                                bool isOnline)
{
    const ridge_regression::Parameter * const parameter = static_cast<const ridge_regression::Parameter *>(par);
    ModelNormEq *rr = static_cast<ModelNormEq *>(r);

    DAAL_INT nRows      = (DAAL_INT)x->getNumberOfRows();     /* vectors */
    DAAL_INT nFeatures  = (DAAL_INT)x->getNumberOfColumns();  /* features */
    DAAL_INT nResponses = (DAAL_INT)y->getNumberOfColumns();  /* variables */
    DAAL_INT nBetas     = (DAAL_INT)rr->getNumberOfBetas();   /* features + 1 */

    DAAL_INT nBetasIntercept = nBetas;
    if (parameter && !parameter->interceptFlag)
    {
        nBetasIntercept--; /* features + 1 */
    }

    DEFINE_TABLE_BLOCK_EX( WriteRows, xtxBlock, rr->getXTXTable().get(), 0, nBetasIntercept );
    DEFINE_TABLE_BLOCK_EX( WriteRows, xtyBlock, rr->getXTYTable().get(), 0, nResponses      );
    DEFINE_TABLE_BLOCK   ( ReadRows,  xBlock,   x );
    DEFINE_TABLE_BLOCK   ( ReadRows,  yBlock,   y );

    algorithmFPType *xtx = xtxBlock.get();
    algorithmFPType *xty = xtyBlock.get();
    algorithmFPType *dx  = const_cast<algorithmFPType *>(xBlock.get());
    algorithmFPType *dy  = const_cast<algorithmFPType *>(yBlock.get());

    /* Initialize output arrays by zero in case of batch mode */
    if(!isOnline)
    {
        daal::services::internal::service_memset<algorithmFPType, cpu>(xtx, 0, nBetasIntercept * nBetasIntercept);
        daal::services::internal::service_memset<algorithmFPType, cpu>(xty, 0, nResponses * nBetasIntercept);
    }

    /* Split rows by blocks */
    size_t numRowsInBlock = 128;

    size_t numBlocks = nRows / numRowsInBlock;
    if (numBlocks * numRowsInBlock < nRows) { numBlocks++; }

    /* Create TLS xtx buffer */
    SafeStatus safeStat;

    daal::tls<algorithmFPType *> xtx_buff( [ =, &safeStat ]()-> algorithmFPType*
    {
        algorithmFPType* xtx_local = service_scalable_calloc<algorithmFPType, cpu>(nBetasIntercept * nBetasIntercept);
        if (!xtx_local) { safeStat.add(services::ErrorMemoryAllocationFailed); }
        return xtx_local;
    } );

    /* Create TLS xty buffer */
    daal::tls<algorithmFPType *> xty_buff( [ =, &safeStat ]()-> algorithmFPType*
    {
        algorithmFPType* xty_local = service_scalable_calloc<algorithmFPType, cpu>(nResponses * nBetasIntercept);
        if (!xty_local) { safeStat.add(services::ErrorMemoryAllocationFailed); }
        return xty_local;
    } );

    daal::threader_for( numBlocks, numBlocks, [ =, &xtx_buff, &xty_buff ](int iBlock)
    {
        algorithmFPType* xtx_local = xtx_buff.local();
        algorithmFPType* xty_local = xty_buff.local();
        if (!xtx_local || !xty_local) { return; }

        size_t startRow = iBlock * numRowsInBlock;
        size_t endRow = startRow + numRowsInBlock;
        if (endRow > nRows) { endRow = nRows; }

        algorithmFPType* dx_ptr = dx + startRow * nFeatures;
        algorithmFPType* dy_ptr = dy + startRow * nResponses;

        DAAL_INT nP = nFeatures;
        DAAL_INT nN = endRow - startRow;
        DAAL_INT nB = nBetasIntercept;
        DAAL_INT nV = nResponses;

        updatePartialSums<algorithmFPType, cpu>(&nP, &nN, &nB, dx_ptr, xtx_local, &nV, dy_ptr, xty_local);
    } );
    DAAL_CHECK_SAFE_STATUS();

    /* Sum all xtx and free buffer */
    xtx_buff.reduce( [ = ](algorithmFPType * v)-> void
    {
       PRAGMA_IVDEP
       PRAGMA_VECTOR_ALWAYS
        for( size_t i = 0; i < (nBetasIntercept * nBetasIntercept); i++)
        {
            xtx[i] += v[i];
        }

        service_scalable_free<algorithmFPType, cpu>( v );
    } );

    /* Sum all xty and free buffer */
    xty_buff.reduce( [ = ](algorithmFPType * v)-> void
    {
       PRAGMA_IVDEP
       PRAGMA_VECTOR_ALWAYS
        for( size_t i = 0; i < (nResponses * nBetasIntercept); i++)
        {
            xty[i] += v[i];
        }

        service_scalable_free<algorithmFPType, cpu>( v );
    } );

    return Status();
} /* updatePartialModelNormEq */

template<typename algorithmFPType, CpuType cpu>
static void copyModelIntermediateTable(NumericTable *srcTable, const algorithmFPType *src,
                                       NumericTable *dstTable,       algorithmFPType *dst)
{
    const size_t srcSize = srcTable->getNumberOfRows() * srcTable->getNumberOfColumns();
    const size_t dstSize = dstTable->getNumberOfRows() * dstTable->getNumberOfColumns();
    daal::services::daal_memcpy_s(dst, dstSize * sizeof(algorithmFPType),
                                  src, srcSize * sizeof(algorithmFPType));
}

template <typename algorithmFPType, CpuType cpu>
Status finalizeModelNormEq(ridge_regression::Model *a,
                           ridge_regression::Model *r,
                           const daal::algorithms::Parameter *par)
{
    ModelNormEq *aa = static_cast<ModelNormEq *>(a);
    ModelNormEq *rr = static_cast<ModelNormEq *>(r);

    DAAL_INT nBetas          = (DAAL_INT)rr->getNumberOfBetas();
    DAAL_INT nResponses      = (DAAL_INT)rr->getNumberOfResponses();
    const bool interceptFlag = rr->getInterceptFlag();
    DAAL_INT nBetasIntercept = nBetas;
    if (!interceptFlag)
    {
        nBetasIntercept--;
    }

    TArray<algorithmFPType, cpu> betaBufferArray(nResponses * nBetas);
    algorithmFPType *betaBuffer = betaBufferArray.get();
    DAAL_CHECK_MALLOC(betaBuffer);

    Status status;
    {
        DEFINE_TABLE_BLOCK_EX( ReadRows, xtxBlock, aa->getXTXTable().get(), 0, nBetas     );
        DEFINE_TABLE_BLOCK_EX( ReadRows, xtyBlock, aa->getXTYTable().get(), 0, nResponses );

        algorithmFPType *xtx = const_cast<algorithmFPType *>(xtxBlock.get());
        algorithmFPType *xty = const_cast<algorithmFPType *>(xtyBlock.get());

        if (aa != rr)
        {
            DEFINE_TABLE_BLOCK( WriteOnlyRows, rxtxBlock, rr->getXTXTable().get() );
            DEFINE_TABLE_BLOCK( WriteOnlyRows, rxtyBlock, rr->getXTYTable().get() );

            NumericTable *aXTX = aa->getXTXTable().get();
            NumericTable *aXTY = aa->getXTYTable().get();
            NumericTable *rXTX = rr->getXTXTable().get();
            NumericTable *rXTY = rr->getXTYTable().get();

            copyModelIntermediateTable<algorithmFPType, cpu>(aXTX, xtx, rXTX, rxtxBlock.get());
            copyModelIntermediateTable<algorithmFPType, cpu>(aXTY, xty, rXTY, rxtyBlock.get());
        }

        const TrainParameter * const trainParameter = static_cast<const TrainParameter *>(par);
        DEFINE_TABLE_BLOCK( ReadRows, ridgeParamsBlock, trainParameter->ridgeParameters.get() );
        algorithmFPType *ridgeParams = const_cast<algorithmFPType *>(ridgeParamsBlock.get());

        const size_t ridgeParamsNumberOfRows = trainParameter->ridgeParameters->getNumberOfRows();
        const bool isEqualRidge = ridgeParamsNumberOfRows == 1;
        status |= computeRidgeCoeffs<algorithmFPType, cpu>(&nBetasIntercept,
                                                           xtx,
                                                           &nBetasIntercept,
                                                           &nResponses,
                                                           xty,
                                                           &nBetasIntercept,
                                                           isEqualRidge,
                                                           interceptFlag,
                                                           ridgeParams,
                                                           betaBuffer);
        DAAL_CHECK_STATUS_VAR(status);
    }

    NumericTable *betaTable = rr->getBeta().get();
    DEFINE_TABLE_BLOCK_EX( WriteOnlyRows, betaBlock, betaTable, 0, nResponses );
    algorithmFPType *beta = betaBlock.get();

    if (nBetasIntercept == nBetas)
    {
        for(size_t i = 0; i < nResponses; i++)
        {
          PRAGMA_IVDEP
          PRAGMA_VECTOR_ALWAYS
            for(size_t j = 1; j < nBetas; j++)
            {
                beta[i * nBetas + j] = betaBuffer[i * nBetas + j - 1];
            }
            beta[i * nBetas] = betaBuffer[i * nBetas + nBetas - 1];
        }
    }
    else
    {
        for(size_t i = 0; i < nResponses; i++)
        {
          PRAGMA_IVDEP
          PRAGMA_VECTOR_ALWAYS
            for(size_t j = 0; j < nBetas - 1; j++)
            {
                beta[i * nBetas + j + 1] = betaBuffer[i * nBetasIntercept + j];
            }
            beta[i * nBetas] = 0.0;
        }
    }

    return status;
} /* finalizeModelNormEq */

template <typename algorithmFPType, CpuType cpu>
Status RidgeRegressionTrainBatchKernel<algorithmFPType, training::normEqDense, cpu>::compute(
    NumericTable *x,
    NumericTable *y,
    ridge_regression::Model *r,
    const daal::algorithms::Parameter *par)
{
    const bool isOnline = false;
    Status status = updatePartialModelNormEq<algorithmFPType, cpu>(x, y, r, par, isOnline);
    DAAL_CHECK_STATUS_VAR(status);

    status |= finalizeModelNormEq<algorithmFPType, cpu>(r, r, par);
    return status;
}

template <typename algorithmfPtype, CpuType cpu>
Status RidgeRegressionTrainOnlineKernel<algorithmfPtype, training::normEqDense, cpu>::compute(
    NumericTable *x,
    NumericTable *y,
    ridge_regression::Model *r,
    const daal::algorithms::Parameter *par)
{
    const bool isOnline = true;
    return updatePartialModelNormEq<algorithmfPtype, cpu>(x, y, r, par, isOnline);
}

template <typename algorithmfPtype, CpuType cpu>
Status RidgeRegressionTrainOnlineKernel<algorithmfPtype, training::normEqDense, cpu>::finalizeCompute(
    ridge_regression::Model *a,
    ridge_regression::Model *r,
    const daal::algorithms::Parameter *par)
{
    return finalizeModelNormEq<algorithmfPtype, cpu>(a, r, par);
}

} // namespace internal
} // namespace training
} // namespace ridge_regression
} // namespace algorithms
} // namespace daal

#endif
