"""
 * Copyright (C) 2021 Axis Communications AB, Lund, Sweden
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
from tf_proto_utils import InferenceClient


# Detector object
# Send images for object detection
class Detector:
    def __init__(self):
        self.debug = None
        self.detection_type = None
        self.inference_client = None
        self.model_path = None
        self.object_list = None
        self.threshold = None

    # Do object detection on image
    def detect(self, image):
        image = np.expand_dims(image, axis=0)
        image = image.astype(np.uint8)
        success, result = self.inference_client.infer({'data': image}, self.model_path)
        if not success:
            return False, 0, 0, 0
        bounding_boxes, classes, confidences = tuple([np.squeeze(result[key]) for key in [
            'TFLite_Detection_PostProcess', 'TFLite_Detection_PostProcess:1', 'TFLite_Detection_PostProcess:2']])

        for i, confidence in enumerate(confidences):
            if confidence < self.threshold:
                print('{} {} found'.format(i, self.detection_type))
                return True, \
                    bounding_boxes[:i], \
                    classes[:i], \
                    confidences[:i]
        return True, bounding_boxes, classes, confidences

    # Load object labels from file
    def read_object_list(self, object_list_path):
        self.object_list = {}
        self.detection_type = 'Objects'
        for row in open(object_list_path, 'r'):
            (classID, label) = row.strip().split(" ", maxsplit=1)
            label = label.strip().split(",", maxsplit=1)[0]
            self.object_list[int(classID)] = label

    # Draw bounding boxes on image
    def draw_bounding_boxes(self, image, bounding_boxes, obj_classes, color=(1, 190, 200)):
        height, width = image.shape[:2]
        for bounding_box, obj_class in zip(bounding_boxes, obj_classes):
            cv2.rectangle(image,
                          (int(bounding_box[1] * width), int(bounding_box[0] * height)),
                          (int(bounding_box[3] * width), int(bounding_box[2] * height)),
                          color,
                          2)
            if self.object_list is not None:
                cv2.putText(image,
                            self.object_list[int(obj_class.item())],
                            (int(bounding_box[1] * width), int(bounding_box[0] * height - 10)),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            1,
                            color,
                            2)
                print(self.object_list[int(obj_class.item())])
        return image

    # Read environment variables
    def read_enviroment(self):
        self.threshold = float(os.environ.get('DETECTION_THRESHOLD', 0.5))
        self.inference_client =  InferenceClient(os.environ['INFERENCE_HOST'])

        self.model_path = os.environ['MODEL_PATH']
        image_path = os.environ.get('IMAGE_PATH')
        object_list_path = os.environ.get('OBJECT_LIST_PATH')
        return image_path, object_list_path

    # Run object detection
    def run(self):
        print(f"object-detector-python connect to: {os.environ['INFERENCE_HOST']}")
        image_path, object_list_path = self.read_enviroment()
        self.read_object_list(object_list_path)
        if image_path is not None:
            while True:
                self.run_image_source(image_path)
        else:
            self.run_camera_source()

    # Run object detection on video stream
    def run_camera_source(self):
        cap = cv2.VideoCapture(1)
        cap.set(cv2.CAP_PROP_FPS, 5)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 480)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 320)
        cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"RGB3"))
        while True:
            _, frame = cap.read()
            succeed, bounding_boxes, obj_classes, _ = self.detect(frame)
            if not succeed:
                time.sleep(1)
                continue
            frame = self.draw_bounding_boxes(frame, bounding_boxes, obj_classes)

    # Run object detection on single image
    def run_image_source(self, image_path):
        image = cv2.imread(image_path)
        succeed = False
        while not succeed:
            t0 = time.time()
            succeed, bounding_boxes, obj_classes, _ = self.detect(image)
            t1 = time.time()
            time.sleep(1)
        image = self.draw_bounding_boxes(image, bounding_boxes, obj_classes)
        cv2.imwrite('/output/{}-detector.jpg'.format(self.detection_type), image)
        print(f'Total time for image inference: {1000 * (t1 - t0):.0f} ms')


# Main program
if __name__ == '__main__':
    Detector().run()
