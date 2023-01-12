#!/usr/bin/env python3
"""
 * Copyright (C) 2022 Axis Communications AB, Lund, Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
"""

# Object detector application example
import os
import time
import numpy as np
import cv2
from flask import Flask, render_template, Response
from tf_proto_utils import InferenceClient

app = Flask(__name__)

@app.route('/')
def index():
    """Video streaming home page."""
    return render_template('index.html')


def gen():
    """Video streaming generator function."""
    detector = Detector()
    yield b'--frame\r\n'
    while True:
        succeed, frame = detector.get_frame()
        if not succeed:
            time.sleep(1)
            continue

        # Convert RGB to BGR format when encoding to keep the correct color
        frame = cv2.imencode('.jpg', cv2.cvtColor(frame, cv2.COLOR_RGB2BGR))[1].tobytes()
        yield b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n--frame\r\n'


@app.route('/video_feed')
def video_feed():
    return Response(gen(), mimetype='multipart/x-mixed-replace; boundary=frame')


class Detector:
    def __init__(self):
        self.debug = None
        self.detection_type = None
        self.inference_client = None
        self.model_path = None
        self.object_list = None
        self.threshold = None
        self.background = None
        self.firs_frame = True
        self.cap = cv2.VideoCapture(1)
        self.cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"RGB3"))
        self.read_environment()

    def draw_connections(self, frame, keypoints, confidences):
        edges = {
            (0, 1): 'm',
            (0, 2): 'c',
            (1, 3): 'm',
            (2, 4): 'c',
            (0, 5): 'm',
            (0, 6): 'c',
            (5, 7): 'm',
            (7, 9): 'm',
            (6, 8): 'c',
            (8, 10): 'c',
            (5, 6): 'y',
            (5, 11): 'm',
            (6, 12): 'c',
            (11, 12): 'y',
            (11, 13): 'm',
            (13, 15): 'm',
            (12, 14): 'c',
            (14, 16): 'c'
        }

        y, x, _ = frame.shape
        # Obtain point positions (int) from float keypoint predictions
        shaped = np.squeeze(np.multiply(keypoints, [y, x]))
        for edge, color in edges.items():
            p1, p2 = edge
            y1, x1 = shaped[p1]
            c1 = confidences[p1]
            y2, x2 = shaped[p2]
            c2 = confidences[p2]

            if (c1 > self.threshold) & (c2 > self.threshold):
                cv2.line(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 0, 255), 4)
        return frame

    def draw_keypoints(self, image, key_points, confidences, color=(1, 190, 200)):
        height, width = image.shape[:2]
        for point, conf in zip(key_points, confidences):
            if conf > self.threshold:
                cv2.circle(image, (int(point[1]*height), int(point[0]*width)), 20, color, thickness=-1, lineType=cv2.FILLED)
        return image

    def draw_prediction(self, image, key_points, confidences):
        output = image.copy()
        output = self.draw_keypoints(output, key_points, confidences)
        output = self.draw_connections(output, key_points, confidences)
        return output

    def detect(self, image):
        image = np.expand_dims(image, axis=0)
        image = image.astype(np.uint8)
        success, result = self.inference_client.infer({'data': image}, self.model_path)
        if not success:
            return False, 0, 0

        key_points = result['StatefulPartitionedCall:0'][0][0][:, :2]
        print("Key points: ", key_points)
        confidences = result['StatefulPartitionedCall:0'][0][0][:, 2]
        print("Confidences: ", confidences)

        return True, key_points, confidences

    # Read environment variables
    def read_environment(self):
        self.threshold = float(os.environ.get('DETECTION_THRESHOLD', 0.3))
        self.inference_client =  InferenceClient(os.environ['INFERENCE_HOST'])
        self.model_path = os.environ['MODEL_PATH']

    # Run object detection on video stream
    def get_frame(self):
        read_success, frame = self.cap.read()
        if not read_success:
            return False, 0
        # Cut the central area of the frame of a 1920x1080 to make it square
        frame = frame[:, 420:-420, :]
        small_frame = cv2.resize(frame, (192, 192), interpolation=cv2.INTER_AREA)
        success, key_points, confidences, = self.detect(small_frame)
        if not success:
            return False, 0

        image = self.draw_prediction(frame, key_points, confidences)
        return True, image


# Main program
if __name__ == '__main__':
    app.run(host='0.0.0.0', threaded=True)
