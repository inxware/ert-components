/* DEPRECATED — ml_model_common.h
 *
 * This file is a compatibility redirect.  The inference engine dispatch layer
 * has been moved to:
 *
 *   target/Component-HAL/ml/engine/ml_inf_engine.h
 *
 * EhsML_Model_Boilerplate_* functions have been renamed EhsML_InfEngine_*.
 * Update your includes and call sites to use ml_inf_engine.h directly.
 */
#include "ml_inf_engine.h"

/* Backwards-compatibility aliases — remove once all callers are updated */
#define EhsML_Model_Boilerplate_Create       EhsML_InfEngine_Create
#define EhsML_Model_Boilerplate_Destroy      EhsML_InfEngine_Destroy
#define EhsML_Model_Boilerplate_SetInputData EhsML_InfEngine_SetInputData
#define EhsML_Model_Boilerplate_RunPipeline   EhsML_InfEngine_RunInference
/** @deprecated signature changed — use EhsML_InfEngine_RunInference(ctx) directly */
#define EhsML_Model_Boilerplate_RunOutputJson EhsML_InfEngine_RunInference
