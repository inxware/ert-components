# This directory contains utilities for building ML models for edge deployment

## Creating a Python Virtual Environment
1. Create python virtual enviroment (optional)
```bash
sudo apt install python3-venv
python3 -m venv venv (Change the second venv here to change the name of the virtual environment you want)
# run this to activate enviroment
source venv/bin/activate
# once done run this do deactivate
deactivate
```

2. Install dependencies

pip install -r requirements.txt

## Creating an Apriltag Dataset

3. Prepare video with apriltags, make sure the video has 1:1 aspect ratio e.g. 640x640, 1080x1080.
   You should be able to crop it using any popular video editing tool, or cofigure camera to record in the above aspect ratio.
   Make sure that the cropped region contains your apriltags.

## 
4. Run traning dataset generator. The script takse two arguments
   <video path> - path to the video file, e.g. ./robot_arm_1080x1080.webm
   <N> - model's input size e.g. '320' (or video N:N resolution e.g. 320x320). It will scale higer video resolution to 'N' regardels of the input video size (as long as it's 1:1)
```bash
./generate_dataset.sh <video path> <N>
```
   Note: Make sure to use stride size of 32, so 'N' should be:
   32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512 …

   For fast models on rpi4  ~16-20ms N=192

5. If run sucesfull it should produce a traing dataset for all apriltags availbe in the video, with folowing structure 
   Where N is the model's input size (or video N:N resolution e.g. 320x320)

#Training and validation data staging
```bash
dataset/xN/
├── images/
│   ├── train/
│   └── val/
├── labels/
│   ├── train/
│   └── val/
├── data.yaml
```

To train a new model obtain the yolo5 repository from ultralitics (pythorch yolo models)
git clone https://github.com/ultralytics/yolov5
cd ./yolov5

# Data labelling
```bash
data.yaml
names: # names of the classes (apriltag+id)
- apriltag{id1}
- apriltag{id2}
...
- apriltag{idX}
nc: n # number of classes (availble IDs in the video)

train: <absolute path>/dataset/xN/images/train
val: <absolute path>/dataset/xN/images/val
```
