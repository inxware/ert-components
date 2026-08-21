// ICB HEADER MACRO START -- DO NOT ALTER
#include "inx-parameters.h"
#include "inx-component.h"
#include "inx-ml_svm_inference.h"
// ICB HEADER MACRO END -- DO NOT ALTER
#include "hal_string.h"
// SVM header and constants
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAXxWIDTH 800
#define MaxSVS 1024

// ICB STATE VAR MACRO START -- DO NOT ALTER
typedef struct inx_ml_svm_inference_state
{
	ehs_sint32 model_type;
	ehs_float *K;          // Diagonal matrix
	double *SV;            // Support vectors
	double alpha[MaxSVS];  // SV multipliers
	double eta;            // Learning rate
	double rho;            // Weight decay
	double epsilon;        // Convergence threshold
	ehs_sint32 epochs;     // Number of epochs
	int xdim;              // Num of dimensions
	ehs_char kernel[128];  // Kernel type
	double kernparm;       // Kernel parameter (if gsn or pol)
	int numsvs;            // Number of SVs
} inx_ml_svm_inference_state_type;
// ICB STATE VAR MACRO END -- DO NOT ALTER

#define INX_ml_svm_inference_ARG_load_model_model_file_path 1
#define INX_ml_svm_inference_ARG_load_model_load_errno 1
#define INX_ml_svm_inference_ARG_load_model_load_ok 1
#define INX_ml_svm_inference_ARG_load_model_load_error 2
#define INX_ml_svm_inference_ARG_do_inference_data 1
#define INX_ml_svm_inference_ARG_do_inference_output 1
#define INX_ml_svm_inference_ARG_do_inference_class_regression 2
#define INX_ml_svm_inference_ARG_do_inference_done_inference 1

// ICB POPULATE EHS DATA STRUCTURE MACRO START -- DO NOT ALTER
EHS_FB_FUNCTIONS_START(ml_svm_inference)
EHS_FB_FUNCTION_ENTRY("load_model", 0x01, ml_svm_inference_load_model)
EHS_FB_FUNCTION_ENTRY("do_inference", 0x02, ml_svm_inference_do_inference)
EHS_FB_FUNCTIONS_END
// ICB POPULATE EHS DATA STRUCTURE MACRO END -- DO NOT ALTER

// Define the kernel function
double compute_kernel(double *SV, double *x, int dim, const char *kernel, double kernparm)
{
	double K_new = 0;
	int j;
	if (EhsStrcmp(kernel, "pol") == 0)
	{
		for (j = 0; j < dim; j++)
		{
			K_new += SV[j] * x[j];
		}
		K_new = pow(K_new + 1, kernparm);
	}
	else if (strcmp(kernel, "lin") == 0)
	{
		for (j = 0; j < dim; j++)
		{
			K_new += SV[j] * x[j];
		}
	}
	else if (strcmp(kernel, "gsn") == 0)
	{
		for (j = 0; j < dim; j++)
		{
			K_new += pow(SV[j] - x[j], 2);
		}
		K_new = exp(-K_new / (2 * pow(kernparm, 2)));
	}
	return K_new;
}

//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_IDENTIFY_FUNCTION(ml_svm_inference)
{
	EHS_FB_IDENTIFY_MEMORY = sizeof(inx_ml_svm_inference_state_type);
}
//ICB IDENTIFY FUNCTION MACRO START -- DO NOT ALTER

// ICB INITIALISE FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_INIT_FUNCTION(ml_svm_inference)
{
	ehs_sint32 model_type;
	ehs_bool bRet = EHS_TRUE; /* assume success */
	inx_ml_svm_inference_state_type* inx_ml_svm_inference_state = (inx_ml_svm_inference_state_type*)EHS_FB_INIT_CONTEXT;
	EhsSscanf(EHS_FB_INIT_PARAMETERS, "%d", &model_type);

	// Initialise parameters
	inx_ml_svm_inference_state->xdim = 2;
	EhsStrcpy(inx_ml_svm_inference_state->kernel, "lin");
	inx_ml_svm_inference_state->kernparm = 2.0;
	inx_ml_svm_inference_state->eta = 0.001;
	inx_ml_svm_inference_state->rho = 0.1;
	inx_ml_svm_inference_state->epsilon = 0.001;
	inx_ml_svm_inference_state->epochs = 1000;
	inx_ml_svm_inference_state->numsvs = 0;

	// Allocate memory
	inx_ml_svm_inference_state->K = (double*)EhsHMem_tempAlloc(MaxSVS * MaxSVS * sizeof(double));
	if (!inx_ml_svm_inference_state->K)
	{
		return EHS_FALSE;
	}
	inx_ml_svm_inference_state->SV = (double*)EhsHMem_tempAlloc(MaxSVS * MAXxWIDTH * sizeof(double));
	if (!inx_ml_svm_inference_state->SV)
	{
		EhsHMem_tempFree(inx_ml_svm_inference_state->K);
		return EHS_FALSE;
	}
	return bRet;
}
// ICB INITIALISE FUNCTION MACRO END -- DO NOT ALTER

// ICB DESTROY FUNCTION MACRO START -- DO NOT ALTER
EHS_FB_DESTROY_FUNCTION(ml_svm_inference)
{
	inx_ml_svm_inference_state_type* inx_ml_svm_inference_state = (inx_ml_svm_inference_state_type*)EHS_FB_DESTROY_CONTEXT;
	EhsHMem_tempFree(inx_ml_svm_inference_state->K);
	EhsHMem_tempFree(inx_ml_svm_inference_state->SV);
	return EHS_TRUE;
}
// ICB DESTROY FUNCTION MACRO END -- DO NOT ALTER

// ICB FUNCTION load_model MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(ml_svm_inference_load_model)
{
	inx_ml_svm_inference_state_type* inx_ml_svm_inference_state = (inx_ml_svm_inference_state_type*)EHS_FB_RUN_CONTEXT;

	inx_ml_svm_inference_state->xdim = 2;
	inx_ml_svm_inference_state->eta = 0.01;
	inx_ml_svm_inference_state->rho = 0.01;
	inx_ml_svm_inference_state->epsilon = 0.00001;
	inx_ml_svm_inference_state->epochs = 10;
	inx_ml_svm_inference_state->kernparm = 1.0;
	EhsStrcpy(inx_ml_svm_inference_state->kernel, "lin");

	// Load SVs and initial alpha values from file
	FILE *infile = NULL;
	if ((infile = Ehs_AppFopen("data.in", "r")) == NULL)
	{
		EHS_FB_FINISH(INX_ml_svm_inference_ARG_load_model_load_error);
		return;
	}

	// Read the first support vector and target value
	for (int i = 0; i < inx_ml_svm_inference_state->xdim; i++)
	{
		if (EhsFscanf(infile, "%lf,", &inx_ml_svm_inference_state->SV[i]) != 1)
		{
			EHS_FB_FINISH(INX_ml_svm_inference_ARG_load_model_load_error);
			fclose(infile);
			return;
		}
	}

	if (EhsSscanf(infile, "%lf,", &inx_ml_svm_inference_state->alpha[0]) != 1)
	{
		EHS_FB_FINISH(INX_ml_svm_inference_ARG_load_model_load_error);
		EhsFclose(infile);
		return;
	}

	inx_ml_svm_inference_state->numsvs = 1; // First support vector loaded

	fclose(infile);
	EHS_FB_FINISH(INX_ml_svm_inference_ARG_load_model_load_ok);
}
// ICB FUNCTION load_model MACRO END -- DO NOT ALTER

// ICB FUNCTION do_inference MACRO START -- DO NOT ALTER
EHS_FB_RUN_FUNCTION(ml_svm_inference_do_inference)
{
	inx_ml_svm_inference_state_type* inx_ml_svm_inference_state = (inx_ml_svm_inference_state_type*)EHS_FB_RUN_CONTEXT;

	ehs_float input_data[MAXxWIDTH] = {1.0, 2.0};
	double prediction = 0;
	double tempdouble;

	// Compute prediction using kernel function
	for (int i = 0; i < inx_ml_svm_inference_state->numsvs; i++)
	{
		tempdouble = compute_kernel(&inx_ml_svm_inference_state->SV[i * MAXxWIDTH], input_data, inx_ml_svm_inference_state->xdim, inx_ml_svm_inference_state->kernel, inx_ml_svm_inference_state->kernparm);
		prediction += inx_ml_svm_inference_state->alpha[i] * tempdouble;
	}

	if (EHS_FB_OUT_CONNECTED_API2(INX_ml_svm_inference_ARG_do_inference_output))
	{
		EhsSnprintf(EHS_FB_OUT_S_API2(INX_ml_svm_inference_ARG_do_inference_output), EHS_FB_OUT_S_CAP_API2(INX_ml_svm_inference_ARG_do_inference_output), "%f", prediction);
	}

	EHS_FB_FINISH(INX_ml_svm_inference_ARG_do_inference_done_inference);
}
// ICB FUNCTION do_inference MACRO END -- DO NOT ALTER
