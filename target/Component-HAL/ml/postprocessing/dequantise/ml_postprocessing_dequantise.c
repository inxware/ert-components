#include "ml_postprocessing_dequantise.h"

void dequantise_box_values(EhsML_Tensor_t quant_tensor, int index, EhsML_Tensor_t dequant_tensor)
{
    // Your dequantisation code here
    float scale = quant_tensor.quantisation_params.scale;
    float zero_point = quant_tensor.quantisation_params.offset;

    for (size_t i = 0; i < dequant_tensor.dims[0]; i++)
    {
        for (size_t j = 0; j < dequant_tensor.dims[1]; j++)
        {
            size_t quant_index = index * quant_tensor.dims[0] + i * dequant_tensor.dims[1] + j;
            ehs_uint8 quantized_value = quant_tensor.data_ptr.u8[quant_index];
            float dequantized_value = ((float)scale) * (((float)quantized_value) - ((float)zero_point));
            // Store the dequantized value in the appropriate position
            dequant_tensor.data_ptr.f32[i * dequant_tensor.dims[1] + j] = dequantized_value;
        }
    }
}