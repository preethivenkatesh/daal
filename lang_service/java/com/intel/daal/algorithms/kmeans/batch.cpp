/* file: batch.cpp */
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

#include <jni.h>
#include "daal.h"
#include "kmeans/JBatch.h"

#include "common_helpers.h"

USING_COMMON_NAMESPACES()
using namespace daal::algorithms::kmeans;

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_kmeans_Batch_cInit
(JNIEnv *, jobject, jint prec, jint method, jlong nClusters, jlong maxIterations)
{
    return jniBatch<kmeans::Method,Batch,lloydDense,lloydCSR>::newObj(prec,method,nClusters,maxIterations);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_kmeans_Batch_cInitParameter
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniBatch<kmeans::Method,Batch,lloydDense,lloydCSR>::getParameter(prec,method,algAddr);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_kmeans_Batch_cGetInput
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniBatch<kmeans::Method,Batch,lloydDense,lloydCSR>::getInput(prec,method,algAddr);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_kmeans_Batch_cGetResult
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniBatch<kmeans::Method,Batch,lloydDense,lloydCSR>::getResult(prec,method,algAddr);
}

JNIEXPORT void JNICALL Java_com_intel_daal_algorithms_kmeans_Batch_cSetResult
(JNIEnv *, jobject, jlong algAddr, jint prec, jint method, jlong resultAddr)
{
    jniBatch<kmeans::Method,Batch,lloydDense,lloydCSR>::setResult<kmeans::Result>(prec,method,algAddr,resultAddr);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_kmeans_Batch_cClone
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniBatch<kmeans::Method,Batch,lloydDense,lloydCSR>::getClone(prec,method,algAddr);
}
