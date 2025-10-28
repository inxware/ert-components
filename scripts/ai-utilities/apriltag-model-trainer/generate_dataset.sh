#!/bin/bash


VIDEO_FILE=$1
MODEL_INPUT_SIZE=$2

# commentout to disable preview
PREVIEW=--preview 

if [ ! -f "$VIDEO_FILE" ]; then
    echo "Must specify video file path as a first argument."
    exit 1
fi

if [ "$MODEL_INPUT_SIZE" = "" ]; then
    echo "Must specify model input size e.g. '320' as a second argument."
    exit 1
fi

DATASET_DIR="dataset/x$MODEL_INPUT_SIZE"

test -d ${DATASET_DIR} && rm -rf ${DATASET_DIR}
mkdir -p ${DATASET_DIR}

python apriltag_dataset_gen.py ${PREVIEW} --video ${VIDEO_FILE} --output ${DATASET_DIR} --frame-step 1 --val-split 0.2 --img-size ${MODEL_INPUT_SIZE}