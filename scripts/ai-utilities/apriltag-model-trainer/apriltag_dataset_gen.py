#!/usr/bin/env python3
"""
apriltag_dataset_gen.py

Convert AprilTag detections in a video into a YOLOv5/Ultralytics⁑ready dataset **with an optional live preview** and a **summary report**.

Usage
-----
python apriltag_dataset_gen.py --video path/to/video.mp4 --output ./dataset --frame-step 5 --val-split 0.2 --preview --img-size 320

Arguments
---------
--video        Input video file.
--output       Output dataset directory (will be created).
--frame-step   Process every N⁑th frame (default 1 = every frame).
--val-split    Fraction of images reserved for validation (default 0.2).
--preview      Show a cv2 window with AprilTag overlays (press **ESC** to quit early).
--img-size     Resize output images to this square size (e.g. 320, 640)

Notes
-----
* The preview drawing is **never** written to the dataset—images saved to `images/` are the pristine frames.
* At the end, a summary lists how many times every tag ID appeared and how many processed frames contained no tags at all.
"""
import cv2
import apriltag
import argparse
import yaml
import random
import sys
from pathlib import Path
from collections import Counter
import numpy as np

def parse_args():
    p = argparse.ArgumentParser(description="Generate YOLOv5 dataset from AprilTags in a video.")
    p.add_argument('--video', required=True, help='Input video file')
    p.add_argument('--output', required=True, help='Output dataset directory')
    p.add_argument('--frame-step', type=int, default=1, help='Process every N-th frame')
    p.add_argument('--val-split', type=float, default=0.2, help='Validation split ratio')
    p.add_argument('--preview', action='store_true', help='Show live preview window with detections')
    p.add_argument('--img-size', type=int, default=0, help='Resize output images to this square size (e.g. 320, 640)')
    return p.parse_args()

def resize_and_pad(frame, img_size):
    h, w = frame.shape[:2]
    scale = min(img_size / w, img_size / h)
    new_w, new_h = int(w * scale), int(h * scale)
    resized = cv2.resize(frame, (new_w, new_h), interpolation=cv2.INTER_AREA)
    pad_x = (img_size - new_w) // 2
    pad_y = (img_size - new_h) // 2
    padded = cv2.copyMakeBorder(resized, pad_y, img_size - new_h - pad_y,
                                pad_x, img_size - new_w - pad_x,
                                cv2.BORDER_CONSTANT, value=(114, 114, 114))
    return padded, scale, pad_x, pad_y

def yolo_bbox_from_corners(corners, img_w, img_h):
    xs = corners[:, 0]
    ys = corners[:, 1]
    x_min, x_max = xs.min(), xs.max()
    y_min, y_max = ys.min(), ys.max()
    x_c = (x_min + x_max) / 2.0
    y_c = (y_min + y_max) / 2.0
    w = x_max - x_min
    h = y_max - y_min
    return x_c / img_w, y_c / img_h, w / img_w, h / img_h

def draw_detection(frame, det, color=(0, 255, 0)):
    pts = det.corners.astype(int)
    for i in range(4):
        cv2.line(frame, tuple(pts[i]), tuple(pts[(i + 1) % 4]), color, 2)
    cx, cy = map(int, det.center)
    cv2.circle(frame, (cx, cy), 3, color, -1)
    cv2.putText(frame, f"id={det.tag_id}", (cx + 5, cy - 5),
                cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2, cv2.LINE_AA)

def main():
    args = parse_args()

    video_path = Path(args.video)
    out_dir    = Path(args.output)

    img_train_dir = out_dir / 'images' / 'train'
    img_val_dir   = out_dir / 'images' / 'val'
    lbl_train_dir = out_dir / 'labels' / 'train'
    lbl_val_dir   = out_dir / 'labels' / 'val'

    for d in [img_train_dir, img_val_dir, lbl_train_dir, lbl_val_dir]:
        d.mkdir(parents=True, exist_ok=True)

    cap = cv2.VideoCapture(str(video_path))
    if not cap.isOpened():
        sys.exit(f"Cannot open video: {video_path}")

    detector = apriltag.Detector()

    tag_to_index = {}
    names = []
    tag_counts = Counter()
    no_tag_frames = 0

    frame_idx = 0
    saved_count = 0

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        if frame_idx % args.frame_step != 0:
            frame_idx += 1
            continue

        orig_frame = frame.copy()
        gray = cv2.cvtColor(orig_frame, cv2.COLOR_BGR2GRAY)
        results = detector.detect(gray)

        if results:
            tag_counts.update([det.tag_id for det in results])

            if args.img_size > 0:
                resized_frame, scale, pad_x, pad_y = resize_and_pad(orig_frame, args.img_size)
                img_h, img_w = args.img_size, args.img_size
            else:
                resized_frame = orig_frame.copy()
                img_h, img_w = orig_frame.shape[:2]
                scale = 1.0
                pad_x = pad_y = 0

            is_val = random.random() < args.val_split
            img_dir = img_val_dir if is_val else img_train_dir
            lbl_dir = lbl_val_dir if is_val else lbl_train_dir

            img_name = f"frame_{frame_idx:06d}.jpg"
            lbl_name = f"frame_{frame_idx:06d}.txt"

            cv2.imwrite(str(img_dir / img_name), resized_frame)

            label_lines = []
            for det in results:
                tag_id = det.tag_id
                if tag_id not in tag_to_index:
                    tag_to_index[tag_id] = len(names)
                    names.append(f"apriltag{tag_id}")
                cls_idx = tag_to_index[tag_id]

                corners = det.corners.copy() * scale
                corners[:, 0] += pad_x
                corners[:, 1] += pad_y

                x, y, w, h = yolo_bbox_from_corners(corners, img_w, img_h)
                label_lines.append(f"{cls_idx} {x:.6f} {y:.6f} {w:.6f} {h:.6f}\n")

            with open(lbl_dir / lbl_name, 'w') as f:
                f.writelines(label_lines)

            saved_count += 1
        else:
            no_tag_frames += 1

        if args.preview:
            preview = orig_frame.copy()
            for det in results:
                draw_detection(preview, det)
            cv2.imshow("AprilTag Preview (ESC to quit)", preview)
            if cv2.waitKey(1) & 0xFF == 27:
                break

        frame_idx += 1

    cap.release()
    if args.preview:
        cv2.destroyAllWindows()

    data_yaml = {
        'train': str(img_train_dir.resolve()),
        'val'  : str(img_val_dir.resolve()),
        'nc'   : len(names),
        'names': names
    }
    with open(out_dir / 'data.yaml', 'w') as f:
        yaml.dump(data_yaml, f)

    print("\n===== Summary =====")
    print(f"Total frames processed : {frame_idx}")
    print(f"Images saved          : {saved_count}")
    for tid, cnt in tag_counts.items():
        print(f"apriltag{tid:<3} detected : {cnt}")
    print(f"Frames with no tags   : {no_tag_frames}")
    print("Dataset directory     :", out_dir)

if __name__ == '__main__':
    main()

