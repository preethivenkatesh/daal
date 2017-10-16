/* file: svd_distr_step3_input.cpp */
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
//  Implementation of svd classes.
//--
*/

#include "algorithms/svd/svd_types.h"
#include "daal_strings.h"

using namespace daal::data_management;
using namespace daal::services;

namespace daal
{
namespace algorithms
{
namespace svd
{
namespace interface1
{

/** Default constructor */
DistributedStep3Input::DistributedStep3Input() : daal::algorithms::Input(lastFinalizeOnLocalInputId + 1) {}

DistributedStep3Input::DistributedStep3Input(const DistributedStep3Input& other) : daal::algorithms::Input(other){}

/**
 * Returns input object for the SVD algorithm
 * \param[in] id    Identifier of the input object
 * \return          Input object that corresponds to the given identifier
 */
DataCollectionPtr DistributedStep3Input::get(FinalizeOnLocalInputId id) const
{
    return staticPointerCast<DataCollection, SerializationIface>(Argument::get(id));
}

/**
 * Sets input object for the SVD algorithm
 * \param[in] id    Identifier of the input object
 * \param[in] value Pointer to the input object value
 */
void DistributedStep3Input::set(FinalizeOnLocalInputId id, const DataCollectionPtr &value)
{
    Argument::set(id, value);
}

Status DistributedStep3Input::getSizes(size_t& nFeatures, size_t& nVectors) const
{
    DataCollectionPtr qCollection = get(inputOfStep3FromStep1);
    DataCollectionPtr rCollection = get(inputOfStep3FromStep2);
    DAAL_CHECK_EX(qCollection, ErrorNullInputDataCollection, ArgumentName, inputOfStep3FromStep2Str());
    DAAL_CHECK_EX(rCollection, ErrorNullInputDataCollection, ArgumentName, inputOfStep3FromStep1Str());

    size_t nodeSize = qCollection->size();
    DAAL_CHECK_EX(nodeSize > 0, ErrorIncorrectNumberOfElementsInInputCollection, ArgumentName, inputOfStep3FromStep2Str());
    DAAL_CHECK_EX(nodeSize == rCollection->size(), ErrorIncorrectNumberOfElementsInInputCollection, ArgumentName, inputOfStep3FromStep1Str());

    DAAL_CHECK_EX((*qCollection)[0], ErrorNullNumericTable, ArgumentName, qCollectionStr());

    NumericTablePtr numTableInQCollection = NumericTable::cast((*qCollection)[0]);
    Status s = checkNumericTable(numTableInQCollection.get(), qCollectionStr());
    if(!s) { return s; }

    nFeatures = numTableInQCollection->getNumberOfColumns();
    nVectors = numTableInQCollection->getNumberOfRows();
    return Status();
}

/**
 * Checks parameters of the algorithm
 * \param[in] parameter Pointer to the parameters
 * \param[in] method Computation method
 */
Status DistributedStep3Input::check(const daal::algorithms::Parameter *parameter, int method) const
{
    DataCollectionPtr qCollection = get(inputOfStep3FromStep1);
    DataCollectionPtr rCollection = get(inputOfStep3FromStep2);
    DAAL_CHECK_EX(qCollection, ErrorNullInputDataCollection, ArgumentName, inputOfStep3FromStep2Str());
    DAAL_CHECK_EX(rCollection, ErrorNullInputDataCollection, ArgumentName, inputOfStep3FromStep1Str());

    size_t nodeSize = qCollection->size();
    size_t nFeatures = 0;
    size_t nVectors = 0;
    Status s = this->getSizes(nFeatures, nVectors);
    if(!s) { return s; }
    for(size_t i = 0 ; i < nodeSize ; i++)
    {
        DAAL_CHECK_EX((*qCollection)[i], ErrorNullNumericTable, ArgumentName, qCollectionStr());
        DAAL_CHECK_EX((*rCollection)[i], ErrorNullNumericTable, ArgumentName, rCollectionStr());

        NumericTablePtr numTableInQCollection = NumericTable::cast((*qCollection)[i]);
        NumericTablePtr numTableInRCollection = NumericTable::cast((*rCollection)[i]);

        DAAL_CHECK_EX(numTableInQCollection, ErrorIncorrectElementInNumericTableCollection, ArgumentName, inputOfStep3FromStep2Str());
        DAAL_CHECK_EX(numTableInRCollection, ErrorIncorrectElementInNumericTableCollection, ArgumentName, inputOfStep3FromStep1Str());

        int unexpectedLayouts = (int)packed_mask;
        s |= checkNumericTable(numTableInQCollection.get(), qCollectionStr(), unexpectedLayouts, 0, nFeatures);
        if(!s) { return s; }
        DAAL_CHECK_EX(numTableInQCollection->getNumberOfRows() >= nFeatures, ErrorNullNumericTable, ArgumentName, rCollectionStr());
        s |= checkNumericTable(numTableInRCollection.get(), rCollectionStr(), unexpectedLayouts, 0, nFeatures, nFeatures);
        if(!s) { return s; }
    }
    return Status();
}

} // namespace interface1
} // namespace svd
} // namespace algorithm
} // namespace daal
