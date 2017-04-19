/* file: implicit_als_predict_ratings_partial_result.cpp */
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
//  Implementation of implicit als algorithm and types methods.
//--
*/

#include "implicit_als_predict_ratings_types.h"
#include "serialization_utils.h"

using namespace daal::data_management;
using namespace daal::services;

namespace daal
{
namespace algorithms
{
namespace implicit_als
{
namespace prediction
{
namespace ratings
{
namespace interface1
{
__DAAL_REGISTER_SERIALIZATION_CLASS(PartialResult, SERIALIZATION_IMPLICIT_ALS_PREDICTION_RATINGS_PARTIAL_RESULT_ID);

/** Default constructor */
PartialResult::PartialResult() : daal::algorithms::PartialResult(1) {}

/**
 * Returns a partial result of the rating prediction stage of the implicit ALS algorithm
 * \param[in] id    Identifier of the partial result
 * \return          Value that corresponds to the given identifier
 */
services::SharedPtr<Result> PartialResult::get(PartialResultId id) const
{
    return services::staticPointerCast<Result, data_management::SerializationIface>(Argument::get(id));
}

/**
 * Sets a partial result of the rating prediction stage of the implicit ALS algorithm
 * \param[in] id    Identifier of the partial result
 * \param[in] ptr   Pointer to the new partial result object
 */
void PartialResult::set(PartialResultId id, const services::SharedPtr<Result> &ptr)
{
    Argument::set(id, ptr);
}

/**
 * Checks a partial result of the implicit ALS algorithm
 * \param[in] input     Pointer to the input object
 * \param[in] parameter %Parameter of the algorithm
 * \param[in] method    Computation method
 */
services::Status PartialResult::check(const daal::algorithms::Input *input, const daal::algorithms::Parameter *parameter, int method) const
{
    SharedPtr<Result> result = get(finalResult);
    const InputIface *algInput = static_cast<const InputIface *>(input);

    size_t nUsers = algInput->getNumberOfUsers();
    size_t nItems = algInput->getNumberOfItems();

    services::Status s;
    int unexpectedLayouts = (int)packed_mask;
    if(result)
    {
        s |= checkNumericTable(result->get(prediction).get(), predictionStr(), unexpectedLayouts, 0, nItems, nUsers);
    }
    return s;
}
}// namespace interface1
}// namespace ratings
}// namespace prediction
}// namespace implicit_als
}// namespace algorithms
}// namespace daal
