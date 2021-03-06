/* file: reshape_layer_types.h */
/*******************************************************************************
* Copyright 2014-2018 Intel Corporation
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
//  Implementation of the absolute value (abs) layer interface
//--
*/

#ifndef __RESHAPE_LAYER_TYPES_H__
#define __RESHAPE_LAYER_TYPES_H__

#include "algorithms/algorithm.h"
#include "data_management/data/tensor.h"
#include "data_management/data/homogen_tensor.h"
#include "services/daal_defines.h"
#include "algorithms/neural_networks/layers/layer_types.h"

namespace daal
{
namespace algorithms
{
namespace neural_networks
{
namespace layers
{
/**
 * @defgroup reshape_layers Reshape Layer
 * \copydoc daal::algorithms::neural_networks::layers::reshape
 * @ingroup layers
 * @{
 */
/**
 * \brief Contains classes of the reshape layer
 */
namespace reshape
{
/**
 * <a name="DAAL-ENUM-ALGORITHMS__NEURAL_NETWORKS__LAYERS__RESHAPE__METHOD"></a>
 * \brief Computation methods for the reshape layer
 */
enum Method
{
    defaultDense = 0 /*!< Default: performance-oriented method */
};

/**
 * <a name="DAAL-ENUM-ALGORITHMS__NEURAL_NETWORKS__LAYERS__RESHAPE__LAYERDATAID"></a>
 * \brief Available identifiers of input objects for the reshape layer
 */
enum LayerDataId
{
    auxInputDimensions = layers::lastLayerInputLayout + 1,  /*!< Numeric table of dimensions of original input data */
    lastLayerDataId = auxInputDimensions
};

/**
* \brief Contains version 1.0 of Intel(R) Data Analytics Acceleration Library (Intel(R) DAAL) interface.
*/
namespace interface1
{
/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__LAYERS__RESHAPE__PARAMETER"></a>
 * \brief Parameters for the reshape layer
 *
 * \snippet neural_networks/layers/reshape/reshape_layer_types.h Parameter source code
 */
/* [Parameter source code] */
struct DAAL_EXPORT Parameter: public layers::Parameter
{
    /**
     * Constructs the parameters of the reshape layer
     */
    Parameter();

    services::Collection<size_t> reshapeDimensions;
};
/* [Parameter source code] */
} // namespace interface1
using interface1::Parameter;

} // namespace reshape
/** @} */
} // namespace layers
} // namespace neural_networks
} // namespace algorithm
} // namespace daal
#endif
