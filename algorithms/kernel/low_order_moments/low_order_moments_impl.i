/* file: low_order_moments_impl.i */
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
//  Implementation of low order moments kernel
//--
*/

#ifndef __LOW_ORDER_MOMENTS_IMPL_I__
#define __LOW_ORDER_MOMENTS_IMPL_I__

#include "low_order_moments_kernel.h"
#include "service_numeric_table.h"
#include "service_error_handling.h"

#include "service_stat.h"
#include "service_math.h"
#include "service_memory.h"
#include "threading.h"


using namespace daal::internal;
using namespace daal::services;

namespace daal
{
namespace algorithms
{
namespace low_order_moments
{
namespace internal
{

/* Multiple instances for defaultDense method optimized implementations */
/* for different estimates sets: all, minmax, meanvariance */
/* Batch and Online(Distributed) variants */

namespace estimates_batch_all
{

#define _THREAD_REDUCTION_
#define _THREAD_FINAL_

#define _MIN_ENABLE_   /*+*/
#define _MAX_ENABLE_   /*+*/
#define _SUM_ENABLE_   /*+*/
#define _SUM2_ENABLE_  /*+*/
#define _SUM2C_ENABLE_ /*+*/
#define _MEAN_ENABLE_  /*+*/
#define _SORM_ENABLE_  /*+*/
#define _VARC_ENABLE_  /*+*/
#define _STDEV_ENABLE_ /*+*/
#define _VART_ENABLE_  /*+*/

#include "low_order_moments_estimates_batch.i"

}

namespace estimates_batch_minmax
{

#define _THREAD_REDUCTION_
#define _THREAD_FINAL_

#define _MIN_ENABLE_   /*+*/
#define _MAX_ENABLE_   /*+*/
#undef  _SUM_ENABLE_
#undef  _SUM2_ENABLE_
#undef  _SUM2C_ENABLE_
#undef  _MEAN_ENABLE_
#undef  _SORM_ENABLE_
#undef  _VARC_ENABLE_
#undef  _STDEV_ENABLE_
#undef  _VART_ENABLE_

#include "low_order_moments_estimates_batch.i"

}

namespace estimates_batch_meanvariance
{

#define _THREAD_REDUCTION_
#define _THREAD_FINAL_

#undef  _MIN_ENABLE_
#undef  _MAX_ENABLE_
#undef  _SUM_ENABLE_
#undef  _SUM2_ENABLE_
#undef  _SUM2C_ENABLE_
#define _MEAN_ENABLE_   /*+*/
#undef  _SORM_ENABLE_
#define _VARC_ENABLE_   /*+*/
#undef  _STDEV_ENABLE_
#undef  _VART_ENABLE_

#include "low_order_moments_estimates_batch.i"

}

namespace estimates_online_all
{

#define _MIN_ENABLE_   /*+*/
#define _MAX_ENABLE_   /*+*/
#define _SUM_ENABLE_   /*+*/
#define _SUM2_ENABLE_  /*+*/
#define _SUM2C_ENABLE_ /*+*/
#define _MEAN_ENABLE_  /*+*/
#define _SORM_ENABLE_  /*+*/
#define _VARC_ENABLE_  /*+*/
#define _STDEV_ENABLE_ /*+*/
#define _VART_ENABLE_  /*+*/

#include "low_order_moments_estimates_online.i"

}

namespace estimates_online_minmax
{

#define _MIN_ENABLE_   /*+*/
#define _MAX_ENABLE_   /*+*/
#undef  _SUM_ENABLE_
#undef  _SUM2_ENABLE_
#undef  _SUM2C_ENABLE_
#undef  _MEAN_ENABLE_
#undef  _SORM_ENABLE_
#undef  _VARC_ENABLE_
#undef  _STDEV_ENABLE_
#undef  _VART_ENABLE_

#include "low_order_moments_estimates_online.i"

}

namespace estimates_online_meanvariance
{

#undef  _MIN_ENABLE_
#undef  _MAX_ENABLE_
#undef  _SUM_ENABLE_
#undef  _SUM2_ENABLE_
#undef  _SUM2C_ENABLE_
#define _MEAN_ENABLE_   /*+*/
#undef  _SORM_ENABLE_
#define _VARC_ENABLE_   /*+*/
#undef  _STDEV_ENABLE_
#undef  _VART_ENABLE_

#include "low_order_moments_estimates_online.i"

}

/****************************************************************************************************************************/
template<Method method>
__int64 getMKLMethod()
{
    __int64 mklMethod = __DAAL_VSL_SS_METHOD_FAST;

    switch(method)
    {
    case defaultDense:
    case fastCSR:
        mklMethod = __DAAL_VSL_SS_METHOD_FAST;
        break;
    case singlePassDense:
    case singlePassCSR:
        mklMethod = __DAAL_VSL_SS_METHOD_1PASS;
        break;
    case sumDense:
    case sumCSR:
        mklMethod = __DAAL_VSL_SS_METHOD_FAST_USER_MEAN;
        break;
    }

    return mklMethod;
}


/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
LowOrderMomentsBatchTask<algorithmFPType, cpu>::LowOrderMomentsBatchTask( NumericTable *dataTable,
                                                                          Result *result) : dataTable(dataTable)
{
    nVectors  = dataTable->getNumberOfRows();
    nFeatures = dataTable->getNumberOfColumns();

    dataTable->getBlockOfRows(0, nVectors, readOnly, dataBD);
    dataBlock = dataBD.getBlockPtr();

    for (size_t i = 0; i < lastResultId + 1; i++)
    {
        resultTable[i] = result->get((ResultId)i);
        resultTable[i]->getBlockOfRows(0, 1, writeOnly, resultBD[i]);
        resultArray[i] = resultBD[i].getBlockPtr();
    }
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
LowOrderMomentsBatchTask<algorithmFPType, cpu>::~LowOrderMomentsBatchTask()
{
    dataTable->releaseBlockOfRows(dataBD);
    for (size_t i = 0; i < lastResultId + 1; i++)
    {
        resultTable[i]->releaseBlockOfRows(resultBD[i]);
    }
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
LowOrderMomentsOnlineTask<algorithmFPType, cpu>::LowOrderMomentsOnlineTask(NumericTable *dataTable)
    : dataTable(dataTable), dataBlock(nullptr), mean(nullptr), raw2Mom(nullptr),
    variance(nullptr), stDev(nullptr), variation(nullptr), prevSums(nullptr)
{
}

template<typename algorithmFPType, CpuType cpu>
Status LowOrderMomentsOnlineTask<algorithmFPType, cpu>::init(PartialResult *partialResult, bool isOnline)
{
    nVectors  = dataTable->getNumberOfRows();
    nFeatures = dataTable->getNumberOfColumns();

    dataTable->getBlockOfRows(0, nVectors, readOnly, dataBD);
    dataBlock = dataBD.getBlockPtr();

    ReadWriteMode rwMode = (isOnline ? readWrite : writeOnly);
    for (size_t i = 0; i < lastPartialResultId + 1; i++)
    {
        resultTable[i] = partialResult->get((PartialResultId)i);
        resultTable[i]->getBlockOfRows(0, 1, rwMode, resultBD[i]);
        resultArray[i] = resultBD[i].getBlockPtr();
    }

    if (!isOnline)
    {
        resultArray[(int)nObservations][0] = 0.0;
    }

    size_t rowSize = nFeatures * sizeof(algorithmFPType);
    mean      = (algorithmFPType *)daal_malloc(rowSize);
    raw2Mom   = (algorithmFPType *)daal_malloc(rowSize);
    variance  = (algorithmFPType *)daal_malloc(rowSize);
    stDev     = (algorithmFPType *)daal_malloc(rowSize);
    variation = (algorithmFPType *)daal_malloc(rowSize);

    DAAL_CHECK(mean && raw2Mom && variance && stDev &&variation, services::ErrorMemoryAllocationFailed);

    if (isOnline)
    {
        prevSums = (algorithmFPType *)daal_malloc(rowSize);
        DAAL_CHECK(prevSums, services::ErrorMemoryAllocationFailed);
        daal_memcpy_s(prevSums, rowSize, resultArray[(int)partialSum], rowSize);
    }
    return Status();
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
LowOrderMomentsOnlineTask<algorithmFPType, cpu>::~LowOrderMomentsOnlineTask()
{
    dataTable->releaseBlockOfRows(dataBD);
    for (size_t i = 0; i < lastPartialResultId + 1; i++)
    {
        resultTable[i]->releaseBlockOfRows(resultBD[i]);
    }

    daal_free(mean);
    daal_free(raw2Mom);
    daal_free(variance);
    daal_free(stDev);
    daal_free(variation);
    if (prevSums) { daal_free(prevSums); }
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
LowOrderMomentsFinalizeTask<algorithmFPType, cpu>::LowOrderMomentsFinalizeTask( NumericTable *nObservationsTable,
                                                                                NumericTable *sumTable,
                                                                                NumericTable *sumSqTable,
                                                                                NumericTable *sumSqCenTable,
                                                                                NumericTable *meanTable,
                                                                                NumericTable *raw2MomTable,
                                                                                NumericTable *varianceTable,
                                                                                NumericTable *stDevTable, NumericTable *variationTable)

                                                                             :  nObservationsTable(nObservationsTable),
                                                                                sumTable(sumTable),
                                                                                sumSqTable(sumSqTable),
                                                                                sumSqCenTable(sumSqCenTable),
                                                                                meanTable(meanTable),
                                                                                raw2MomTable(raw2MomTable),
                                                                                varianceTable(varianceTable),
                                                                                stDevTable(stDevTable),
                                                                                variationTable(variationTable)
{
    nFeatures = sumTable->getNumberOfColumns();

    nObservationsTable->getBlockOfRows(0, 1, readOnly, nObservationsBD);
    nObservations = nObservationsBD.getBlockPtr();

    sumTable     ->getBlockOfRows(0, 1, readOnly, sumBD);
    sumSqTable   ->getBlockOfRows(0, 1, readOnly, sumSqBD);
    sumSqCenTable->getBlockOfRows(0, 1, readOnly, sumSqCenBD);

    sums     = sumBD     .getBlockPtr();
    sumSq    = sumSqBD   .getBlockPtr();
    sumSqCen = sumSqCenBD.getBlockPtr();

    meanTable     ->getBlockOfRows(0, 1, writeOnly, meanBD);
    raw2MomTable  ->getBlockOfRows(0, 1, writeOnly, raw2MomBD);
    varianceTable ->getBlockOfRows(0, 1, writeOnly, varianceBD);
    stDevTable    ->getBlockOfRows(0, 1, writeOnly, stDevBD);
    variationTable->getBlockOfRows(0, 1, writeOnly, variationBD);

    mean      = meanBD     .getBlockPtr();
    raw2Mom   = raw2MomBD  .getBlockPtr();
    variance  = varianceBD .getBlockPtr();
    stDev     = stDevBD    .getBlockPtr();
    variation = variationBD.getBlockPtr();
}

template<typename algorithmFPType, CpuType cpu>
/****************************************************************************************************************************/
LowOrderMomentsFinalizeTask<algorithmFPType, cpu>::~LowOrderMomentsFinalizeTask()
{
    nObservationsTable->releaseBlockOfRows(nObservationsBD);
    sumTable          ->releaseBlockOfRows(sumBD);
    sumSqTable        ->releaseBlockOfRows(sumSqBD);
    sumSqCenTable     ->releaseBlockOfRows(sumSqCenBD);
    meanTable         ->releaseBlockOfRows(meanBD);
    raw2MomTable      ->releaseBlockOfRows(raw2MomBD);
    varianceTable     ->releaseBlockOfRows(varianceBD);
    stDevTable        ->releaseBlockOfRows(stDevBD);
    variationTable    ->releaseBlockOfRows(variationBD);
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
Status retrievePrecomputedStatsIfPossible( size_t nFeatures,
                                         size_t nVectors,
                                         NumericTable *dataTable,
                                         algorithmFPType *sums,
                                         algorithmFPType *mean)
{
    NumericTable *precomputedSumsTable = dataTable->basicStatistics.get(NumericTable::sum).get();

    if (!precomputedSumsTable)
        return Status(services::ErrorPrecomputedSumNotAvailable);

    BlockDescriptor<algorithmFPType> precomputedSumsBlock;
    precomputedSumsTable->getBlockOfRows(0, 1, readOnly, precomputedSumsBlock);
    algorithmFPType *precomputedSums = precomputedSumsBlock.getBlockPtr();

    size_t rowSize = nFeatures * sizeof(algorithmFPType);
    daal_memcpy_s(sums, rowSize, precomputedSums, rowSize);

    precomputedSumsTable->releaseBlockOfRows(precomputedSumsBlock);

    algorithmFPType invNVectors = 1.0 / (algorithmFPType)nVectors;

    for (size_t i = 0; i < nFeatures; i++)
    {
        mean[i] = sums[i] * invNVectors;
    }
    return Status();
}


/****************************************************************************************************************************/
template<typename algorithmFPType, Method method, CpuType cpu>
Status computeSumAndVariance(size_t nFeatures,
                            size_t nVectors,
                            algorithmFPType *dataBlock,
                            algorithmFPType *sums,
                            algorithmFPType *prevSums,
                            algorithmFPType *mean,
                            algorithmFPType *raw2Mom,
                            algorithmFPType *variance,
                            bool isOnline)
{
    algorithmFPType nPreviousObservations = 0.0;

    const __int64 mklMethod = getMKLMethod<method>();

    const int mklStatus = Statistics<algorithmFPType, cpu>::xSumAndVariance( dataBlock,
                                                                       nFeatures,
                                                                       nVectors,
                                                                       &nPreviousObservations,
                                                                       mklMethod,
                                                                       sums,
                                                                       mean,
                                                                       raw2Mom,
                                                                       variance );

    if (mklStatus != 0)
        return Status(services::ErrorLowOrderMomentsInternal);

    if (isOnline)
    {
      PRAGMA_IVDEP
        for(size_t i = 0; i < nFeatures; i++)
        {
            sums[i] += prevSums[i];
        }
    }

    return Status();
}

/****************************************************************************************************************************/
template<typename algorithmFPType, Method method, CpuType cpu>
Status computeSum_Mean_SecondOrderRawMoment_Variance_Variation( size_t nFeatures,
                                                              size_t nVectors,
                                                              algorithmFPType *dataBlock,
                                                              algorithmFPType *sums,
                                                              algorithmFPType *mean,
                                                              algorithmFPType *raw2Mom,
                                                              algorithmFPType *variance,
                                                              algorithmFPType *variation)
{
    const __int64 mklMethod = getMKLMethod<method>();

    const int mklStatus = Statistics<algorithmFPType, cpu>::xLowOrderMoments(dataBlock,
                                                                        nFeatures,
                                                                        nVectors,
                                                                        mklMethod,
                                                                        sums,
                                                                        mean,
                                                                        raw2Mom,
                                                                        variance,
                                                                        variation );

    return mklStatus ? Status(services::ErrorLowOrderMomentsInternal) : Status();
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void initializeMinAndMax( size_t nFeatures,
                          const algorithmFPType *dataBlock,
                          algorithmFPType *min,
                          algorithmFPType *max )
{
    size_t rowSize = nFeatures * sizeof(algorithmFPType);

    daal_memcpy_s(min, rowSize, dataBlock, rowSize);
    daal_memcpy_s(max, rowSize, dataBlock, rowSize);
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void computeMinAndMax( size_t nFeatures,
                       size_t nVectors,
                       algorithmFPType *dataBlock,
                       algorithmFPType *min,
                       algorithmFPType *max )
{
    algorithmFPType *dataPtr = dataBlock;

    for(size_t i = 0; i < nVectors; i++, dataPtr += nFeatures)
    {
        for(size_t j = 0; j < nFeatures; j++)
        {
            if (dataPtr[j] < min[j])
            {
                min[j] = dataPtr[j];
            }

            if (dataPtr[j] > max[j])
            {
                max[j] = dataPtr[j];
            }
        }
    }
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void computeSumOfSquares( size_t nFeatures,
                          size_t nVectors,
                          algorithmFPType *dataBlock,
                          algorithmFPType *sumSq,
                          bool isOnline )
{
    if (!isOnline)
    {
        const algorithmFPType zero = 0.0;
        for(size_t j = 0; j < nFeatures; j++)
        {
            sumSq[j] = zero;
        }
    }

    algorithmFPType *dataPtr = dataBlock;

    for(size_t i = 0; i < nVectors; i++, dataPtr += nFeatures)
    {
      PRAGMA_IVDEP
        for(size_t j = 0; j < nFeatures; j++)
        {
            sumSq[j] += dataPtr[j] * dataPtr[j];
        }
    }
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void computeSumOfSquaredDiffsFromMean( size_t nFeatures,
                                       size_t nVectors,
                                       size_t prevNVectors,
                                       algorithmFPType *variance,
                                       algorithmFPType *sums,
                                       algorithmFPType *prevSums,
                                       algorithmFPType *sumSqCen,
                                       bool isOnline )
{
    if (nVectors > 0)
    {
        algorithmFPType nVectorsM1 = (algorithmFPType)(nVectors - 1);
        if (!isOnline)
        {
            for (size_t i = 0; i < nFeatures; i++)
            {
                sumSqCen[i] = variance[i] * nVectorsM1;
            }
            return;
        }

        if (prevNVectors == 0)
        {
            for (size_t i = 0; i < nFeatures; i++)
            {
                sumSqCen[i] += variance[i] * nVectorsM1;
            }
        }
        else
        {
            algorithmFPType invPrevNVectors = 1.0 / (algorithmFPType)prevNVectors;
            algorithmFPType invNVectors = 1.0 / (algorithmFPType)nVectors;
            algorithmFPType coeff = (algorithmFPType)(prevNVectors * nVectors) / (algorithmFPType)(prevNVectors + nVectors);

            for (size_t i = 0; i < nFeatures; i++)
            {
                algorithmFPType mean1 = prevSums[i] * invPrevNVectors;
                algorithmFPType mean2 = (sums[i] - prevSums[i]) * invNVectors;

                algorithmFPType ssqdm2 = variance[i] * nVectorsM1;

                sumSqCen[i] += (ssqdm2 + coeff * (mean1 * mean1 + mean2 * mean2 - 2 * mean1 * mean2));
            }
        }
    }
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void mergeNObservations( data_management::DataCollection *partialResultsCollection,
                         PartialResult *partialResult,
                         int *partialNObservations )
{
    PartialResult *inputPartialResult = static_cast<PartialResult* >((*partialResultsCollection)[0].get());

    NumericTable *nObsTable = partialResult->get(nObservations).get();
    NumericTable *inputNObsTable = inputPartialResult->get(nObservations).get();

    BlockDescriptor<int> nObsBD, inputNObsBD;

    nObsTable->getBlockOfRows(0, 1, writeOnly, nObsBD);
    inputNObsTable->getBlockOfRows(0, 1, readOnly, inputNObsBD);
    int *nObs = nObsBD.getBlockPtr();
    int *inputNObs = inputNObsBD.getBlockPtr();

    nObs[0] = inputNObs[0];
    partialNObservations[0] = inputNObs[0];

    inputNObsTable->releaseBlockOfRows(inputNObsBD);
    size_t collectionSize = partialResultsCollection->size();

    for (size_t i = 1; i < collectionSize; i++)
    {
        inputPartialResult = static_cast<PartialResult* >((*partialResultsCollection)[i].get());
        inputNObsTable = inputPartialResult->get(nObservations).get();
        inputNObsTable->getBlockOfRows(0, 1, readOnly, inputNObsBD);
        int *inputNObs = inputNObsBD.getBlockPtr();

        nObs[0] += inputNObs[0];
        partialNObservations[i] = inputNObs[0];

        inputNObsTable->releaseBlockOfRows(inputNObsBD);
    }
    nObsTable->releaseBlockOfRows(nObsBD);
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void getTwoTables( ReadWriteMode rwMode,
                   NumericTable *table1,
                   NumericTable *table2,
                   BlockDescriptor<algorithmFPType> &bd1,
                   BlockDescriptor<algorithmFPType> &bd2,
                   algorithmFPType **array1,
                   algorithmFPType **array2 )
{
    table1->getBlockOfRows(0, 1, rwMode, bd1);
    table2->getBlockOfRows(0, 1, rwMode, bd2);
    *array1 = bd1.getBlockPtr();
    *array2 = bd2.getBlockPtr();
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void releaseTwoTables( NumericTable *table1,
                       NumericTable *table2,
                       BlockDescriptor<algorithmFPType> &bd1,
                       BlockDescriptor<algorithmFPType> &bd2 )
{
    table1->releaseBlockOfRows(bd1);
    table2->releaseBlockOfRows(bd2);
}
/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void mergeMinAndMax( data_management::DataCollection *partialResultsCollection,
                     PartialResult *partialResult )
{
    NumericTable *minTable = partialResult->get(partialMinimum).get();
    NumericTable *maxTable = partialResult->get(partialMaximum).get();

    size_t nFeatures = minTable->getNumberOfColumns();

    BlockDescriptor<algorithmFPType> minBD, maxBD;
    algorithmFPType *min, *max;

    getTwoTables<algorithmFPType, cpu>( writeOnly,
                                        minTable,
                                        maxTable,
                                        minBD,
                                        maxBD,
                                        &min,
                                        &max);

    PartialResult *inputPartialResult = static_cast<PartialResult* >((*partialResultsCollection)[0].get());
    NumericTable *inputMinTable = inputPartialResult->get(partialMinimum).get();
    NumericTable *inputMaxTable = inputPartialResult->get(partialMaximum).get();

    BlockDescriptor<algorithmFPType> inputMinBD, inputMaxBD;
    algorithmFPType *inputMin, *inputMax;

    getTwoTables<algorithmFPType, cpu>( readOnly,
                                        inputMinTable,
                                        inputMaxTable,
                                        inputMinBD,
                                        inputMaxBD,
                                        &inputMin,
                                        &inputMax );

    size_t rowSize = nFeatures * sizeof(algorithmFPType);
    daal_memcpy_s(min, rowSize, inputMin, rowSize);
    daal_memcpy_s(max, rowSize, inputMax, rowSize);

    releaseTwoTables<algorithmFPType, cpu>( inputMinTable,
                                            inputMaxTable,
                                            inputMinBD,
                                            inputMaxBD );

    size_t collectionSize = partialResultsCollection->size();

    for (size_t i = 1; i < collectionSize; ++i)
    {
        inputPartialResult = static_cast<PartialResult* >((*partialResultsCollection)[i].get());
        inputMinTable = inputPartialResult->get(partialMinimum).get();
        inputMaxTable = inputPartialResult->get(partialMaximum).get();

        getTwoTables<algorithmFPType, cpu>( readOnly,
                                            inputMinTable,
                                            inputMaxTable,
                                            inputMinBD,
                                            inputMaxBD,
                                            &inputMin,
                                            &inputMax );

        for (size_t j = 0; j < nFeatures; ++j)
        {
            if (inputMin[j] < min[j]) { min[j] = inputMin[j]; }
            if (inputMax[j] > max[j]) { max[j] = inputMax[j]; }
        }

        releaseTwoTables<algorithmFPType, cpu>( inputMinTable,
                                                inputMaxTable,
                                                inputMinBD,
                                                inputMaxBD );
    }

    releaseTwoTables<algorithmFPType, cpu>( minTable,
                                            maxTable,
                                            minBD,
                                            maxBD );
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void getThreeTables( ReadWriteMode rwMode,
                     NumericTable *table1,
                     NumericTable *table2,
                     NumericTable *table3,
                     BlockDescriptor<algorithmFPType> &bd1,
                     BlockDescriptor<algorithmFPType> &bd2,
                     BlockDescriptor<algorithmFPType> &bd3,
                     algorithmFPType **array1,
                     algorithmFPType **array2,
                     algorithmFPType **array3 )
{
    table1->getBlockOfRows(0, 1, rwMode, bd1);
    table2->getBlockOfRows(0, 1, rwMode, bd2);
    table3->getBlockOfRows(0, 1, rwMode, bd3);
    *array1 = bd1.getBlockPtr();
    *array2 = bd2.getBlockPtr();
    *array3 = bd3.getBlockPtr();
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void releaseThreeTables( NumericTable *table1,
                         NumericTable *table2,
                         NumericTable *table3,
                         BlockDescriptor<algorithmFPType> &bd1,
                         BlockDescriptor<algorithmFPType> &bd2,
                         BlockDescriptor<algorithmFPType> &bd3 )
{
    table1->releaseBlockOfRows(bd1);
    table2->releaseBlockOfRows(bd2);
    table3->releaseBlockOfRows(bd3);
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void mergeSums( data_management::DataCollection *partialResultsCollection,
                PartialResult *partialResult,
                int *partialNObservations )
{

    ///merge first block
    size_t nMergedObservations = partialNObservations[0];
    NumericTable *sumTable      = partialResult->get(partialSum).get();
    NumericTable *sumSqTable    = partialResult->get(partialSumSquares).get();
    NumericTable *sumSqCenTable = partialResult->get(partialSumSquaresCentered).get();

    size_t nFeatures = sumTable->getNumberOfColumns();

    BlockDescriptor<algorithmFPType> sumBD, sumSqBD, sumSqCenBD;
    algorithmFPType *sums, *sumSq, *sumSqCen;

    getThreeTables<algorithmFPType, cpu>( writeOnly,
                                          sumTable,
                                          sumSqTable,
                                          sumSqCenTable,
                                          sumBD,
                                          sumSqBD,
                                          sumSqCenBD,
                                          &sums,
                                          &sumSq,
                                          &sumSqCen );

    PartialResult *inputPartialResult = static_cast<PartialResult* >((*partialResultsCollection)[0].get());

    NumericTable *inputSumTable      = inputPartialResult->get(partialSum).get();
    NumericTable *inputSumSqTable    = inputPartialResult->get(partialSumSquares).get();
    NumericTable *inputSumSqCenTable = inputPartialResult->get(partialSumSquaresCentered).get();

    BlockDescriptor<algorithmFPType> inputSumBD, inputSumSqBD, inputSumSqCenBD;
    algorithmFPType *inputSums, *inputSumSq, *inputSumSqCen;

    getThreeTables<algorithmFPType, cpu>( readOnly,
                                          inputSumTable,
                                          inputSumSqTable,
                                          inputSumSqCenTable,
                                          inputSumBD,
                                          inputSumSqBD,
                                          inputSumSqCenBD,
                                          &inputSums,
                                          &inputSumSq,
                                          &inputSumSqCen );

    size_t rowSize = nFeatures * sizeof(algorithmFPType);
    daal_memcpy_s(sums,     rowSize, inputSums,     rowSize);
    daal_memcpy_s(sumSq,    rowSize, inputSumSq,    rowSize);
    daal_memcpy_s(sumSqCen, rowSize, inputSumSqCen, rowSize);

    releaseThreeTables<algorithmFPType, cpu>( inputSumTable,
                                              inputSumSqTable,
                                              inputSumSqCenTable,
                                              inputSumBD,
                                              inputSumSqBD,
                                              inputSumSqCenBD );

    size_t collectionSize = partialResultsCollection->size();

    BlockDescriptor<int> inputNObsBD;
    int *inputNObs;
    for(size_t block = 1; block < collectionSize; ++block)
    {
        inputPartialResult = static_cast<PartialResult* >((*partialResultsCollection)[block].get());

        inputSumTable      = inputPartialResult->get(partialSum).get();
        inputSumSqTable    = inputPartialResult->get(partialSumSquares).get();
        inputSumSqCenTable = inputPartialResult->get(partialSumSquaresCentered).get();

        getThreeTables<algorithmFPType, cpu>(readOnly, inputSumTable, inputSumSqTable, inputSumSqCenTable,
            inputSumBD, inputSumSqBD, inputSumSqCenBD, &inputSums, &inputSumSq, &inputSumSqCen);

        int n1 = nMergedObservations;
        int n2 = partialNObservations[block];

        if(n2 == 0)
        {
            continue;
        }

        if(n1 == 0)
        {
            for(size_t i = 0; i < nFeatures; i++)
            {
                sumSqCen[i] += inputSumSqCen[i];
                sumSq[i]    += inputSumSq[i];
                sums[i]     += inputSums[i];
            }
            nMergedObservations += n2;
            continue;
        }

        algorithmFPType coeff = (algorithmFPType)(n1 * n2) / (algorithmFPType)(n1 + n2);
        algorithmFPType invN1 = 1.0 / (algorithmFPType)n1;
        algorithmFPType invN2 = 1.0 / (algorithmFPType)n2;

        for(size_t i = 0; i < nFeatures; i++)
        {
            algorithmFPType mean1 = sums[i] * invN1;
            algorithmFPType mean2 = inputSums[i] * invN2;

            sumSqCen[i] += (inputSumSqCen[i] + coeff * (mean1 * mean1 + mean2 * mean2 - 2 * mean1 * mean2));

            sums[i]  += inputSums[i];
            sumSq[i] += inputSumSq[i];
        }
        nMergedObservations += n2;

        releaseThreeTables<algorithmFPType, cpu>(inputSumTable, inputSumSqTable, inputSumSqCenTable,
            inputSumBD, inputSumSqBD, inputSumSqCenBD);
    }

    releaseThreeTables<algorithmFPType, cpu>(sumTable, sumSqTable, sumSqCenTable,
        sumBD, sumSqBD, sumSqCenBD);
}

/****************************************************************************************************************************/
template<typename algorithmFPType, CpuType cpu>
void finalize( LowOrderMomentsFinalizeTask<algorithmFPType, cpu> &task )
{
    algorithmFPType invNObservations   = 1.0 / (algorithmFPType)(task.nObservations[0]);
    algorithmFPType invNObservationsM1 = 1.0 / (algorithmFPType)(task.nObservations[0] - 1);

    algorithmFPType *sums     = task.sums;
    algorithmFPType *sumSq    = task.sumSq;
    algorithmFPType *sumSqCen = task.sumSqCen;

    algorithmFPType *mean      = task.mean;
    algorithmFPType *raw2Mom   = task.raw2Mom;
    algorithmFPType *variance  = task.variance;
    algorithmFPType *stDev     = task.stDev;
    algorithmFPType *variation = task.variation;

    size_t nFeatures           = task.nFeatures;

   PRAGMA_IVDEP
   PRAGMA_VECTOR_ALWAYS
   PRAGMA_VECTOR_ALWAYS
    for (size_t i = 0; i < nFeatures; i++)
    {
        mean[i]      = sums[i]     * invNObservations;
        raw2Mom[i]   = sumSq[i]    * invNObservations;
        variance[i]  = sumSqCen[i] * invNObservationsM1;
        stDev[i]     = daal::internal::Math<algorithmFPType, cpu>::sSqrt(variance[i]);
        variation[i] = stDev[i] / mean[i];
    }
}

}
}
}
}

#endif
