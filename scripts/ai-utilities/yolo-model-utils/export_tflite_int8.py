import sys
from pathlib import Path
import tensorflow as tf
import numpy as np

if len(sys.argv) != 2:
    print("Usage: python export_tflite_int8.py <weights_dir>")
    sys.exit(1)

path = Path(sys.argv[1])
saved_model_dir = path / "best_saved_model"
output_path = path / "best_int8.tflite"

# Verify SavedModel exists
if not saved_model_dir.exists():
    print(f"Error: SavedModel directory not found: {saved_model_dir}")
    sys.exit(1)

# Load the model to extract input shape
model = tf.saved_model.load(str(saved_model_dir))
concrete_func = model.signatures["serving_default"]
input_tensor = list(concrete_func.structured_input_signature[1].values())[0]
input_shape = input_tensor.shape.as_list()

# Define TFLite converter
converter = tf.lite.TFLiteConverter.from_saved_model(str(saved_model_dir))
converter.optimizations = [tf.lite.Optimize.DEFAULT]

# Representative dataset using extracted input shape
def representative_data_gen():
    for _ in range(100):
        yield [np.random.rand(*input_shape).astype(np.float32)]

converter.representative_dataset = representative_data_gen
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.uint8
converter.inference_output_type = tf.uint8

# Convert and save
tflite_model = converter.convert()
with open(output_path, "wb") as f:
    f.write(tflite_model)

print(f"INT8 TFLite model saved to: {output_path}")
