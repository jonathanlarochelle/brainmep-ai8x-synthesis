#!/bin/sh
./ai8xize.py --verbose -L --top-level cnn --test-dir tests/tensorflow --prefix simpleNet3 --checkpoint-file ../ai8x-training/TensorFlow/test/SimpleNet3/saved_model/saved_model.onnx --config-file tests/tensorflow/simple-net3.yaml --sample-input ../ai8x-training/TensorFlow/test/SimpleNet3/saved_model/input_sample_1x3x5.npy --device 85 --compact-data --mexpress --unload --embedded-code --keep-first --scale 1.0 "$@"
