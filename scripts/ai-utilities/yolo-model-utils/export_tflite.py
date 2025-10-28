import sys
import tensorflow as tf
from pathlib import Path

if len(sys.argv) != 2:
    print("Usage: python export_tflite.py <weights_dir>")
    sys.exit(1)

path = Path(sys.argv[1])
saved_model_dir = path / "best_saved_model"
output_path = path / "best.tflite"

if not saved_model_dir.exists():
    print(f"Error: SavedModel directory not found: {saved_model_dir}")
    sys.exit(1)

converter = tf.lite.TFLiteConverter.from_saved_model(str(saved_model_dir))
converter.optimizations = [tf.lite.Optimize.DEFAULT]  # Optional
tflite_model = converter.convert()

with open(output_path, "wb") as f:
    f.write(tflite_model)

print(f"TFLite model saved to: {output_path}")
