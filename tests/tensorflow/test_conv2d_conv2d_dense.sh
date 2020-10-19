#!/bin/sh
./ai8xize.py --verbose -L --top-level cnn --test-dir tests/tensorflow --prefix covn2d_conv2d_dense --checkpoint-file ../ai8x-training/TensorFlow/test/Conv2D_Conv2D_Dense/saved_model/saved_model.onnx --config-file tests/tensorflow/simple-conv2d-conv2d-dense.yaml --sample-input ../ai8x-training/TensorFlow/test/Conv2D_Conv2D_Dense/saved_model/input_sample_3x2x5.npy --device 85 --compact-data --mexpress --unload --embedded-code --keep-first --scale 1.0 "$@"
