#ifndef INXWARE_ml_svm_inference
#define INXWARE_ml_svm_inference
#include "inx-component.h"

/*****************************************************
 * Support Vector Machine (SVM) inference function block.
 * Loads a support vector model from file and runs
 * kernel-based classification or regression inference.
 * Supports linear, polynomial and Gaussian kernels.
 *****************************************************/
#define INXWARE_FB_ID_ml_svm_inference 0x533F
#define INXWARE_FB_NAME_ml_svm_inference "ml_svm_inference"

/* Declaring the functions used here as public interface for static linking into EHS builds */
EHS_COMPONENT_API_EXPORT EHS_FB_IDENTIFY_FUNCTION(ml_svm_inference);         // this relates to C file identify function
EHS_COMPONENT_API_EXPORT EHS_FB_INIT_FUNCTION(ml_svm_inference);             // this relates to C-file init function
EHS_COMPONENT_API_EXPORT EHS_FB_DESTROY_FUNCTION(ml_svm_inference);          // this relates to C-file destroy function
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ml_svm_inference_load_model);   // a function executed by a start port
EHS_COMPONENT_API_EXPORT EHS_FB_RUN_FUNCTION(ml_svm_inference_do_inference); // a function executed by a start port
/* declare the table for use by the blockref table */
EHS_FB_FUNCTIONS(ml_svm_inference)

#endif /* INXWARE_ml_svm_inference */
