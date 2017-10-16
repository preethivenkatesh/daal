/* file: GroupOfBetasParameter.java */
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

/**
 * @ingroup linear_regression_quality_metric_group_of_betas
 * @{
 */
/**
 * @brief Contains classes for computing the single beta metric */
package com.intel.daal.algorithms.linear_regression.quality_metric;

import com.intel.daal.services.DaalContext;

/**
 * <a name="DAAL-CLASS-ALGORITHMS__LINEAR_REGRESSION__QUALITY_METRIC__GROUPOFBETASPARAMETER"></a>
 * @brief Base class for the parameters of the algorithm
 */
public class GroupOfBetasParameter extends com.intel.daal.algorithms.Parameter {

    /**
     * Constructs the parameters of the quality metric algorithm
     * @param context   Context to manage the parameters of the quality metric algorithm
     */
    public GroupOfBetasParameter(DaalContext context) {
        super(context);
    }

    public GroupOfBetasParameter(DaalContext context, long cParameter) {
        super(context, cParameter);
    }

    /**
     *  Gets the number of beta coefficients
     *  @return  Number of betas
     */
    public long getNBeta() {
        return cGetNBeta(this.cObject);
    }

    /**
     *  Sets the number of beta coefficients
     *  @param nBeta Number of beta coefficients in the model
     */
    public void setNBeta(long nBeta) {
        cSetNBeta(this.cObject, nBeta);
    }

    /**
     *  Gets the number of beta coefficients in reduced model
     *  @return  Number of betas in reduced model
     */
    public long getNBetaReducedModel() {
        return cGetNBetaReducedModel(this.cObject);
    }

    /**
     *  Sets the number of beta coefficients
     *  @param nBetaReducedModel Number of beta coefficients in the model
     */
    public void setNBetaReducedModel(long nBetaReducedModel) {
        cSetNBetaReducedModel(this.cObject, nBetaReducedModel);
    }

    /**
     * Retrieves the accuracy of the algorithm (used for statistics calculation)
     * @return Accuracy of the algorithm
     */
    public double getAccuracyThreshold() {
        return cGetAccuracyThreshold(this.cObject);
    }

    /**
     * Sets the accuracy of the algorithm (used for statistics calculation)
     * @param accuracyThreshold Accuracy of the algorithm
     */
    public void setAccuracyThreshold(double accuracyThreshold) {
        cSetAccuracyThreshold(this.cObject, accuracyThreshold);
    }

    private native void cSetNBeta(long parAddr, long nBeta);
    private native long cGetNBeta(long parAddr);
    private native void cSetNBetaReducedModel(long parAddr, long nBetaReducedModel);
    private native long cGetNBetaReducedModel(long parAddr);
    private native void cSetAccuracyThreshold(long parAddr, double acc);
    private native double cGetAccuracyThreshold(long parAddr);
}
/** @} */
