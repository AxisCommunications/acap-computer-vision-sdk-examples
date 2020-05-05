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
        self.model_input_height = None
        self.model_input_width = None
        self.model_name = None
        self.object_list = None
        self.threshold = None

    # Do object detection on image
    def detect(self, image):
        image, width_mul, height_mul = rescale(image, self.model_input_width, self.model_input_height)
        image = np.expand_dims(image, axis=0)
        image = image.astype(np.uint8)
        success, result = self.inference_client.infer({'data': image}, self.model_name)
        if not success:
            return False, 0, 0, 0
        bounding_boxes, classes, confidences = tuple([np.squeeze(result[key]) for key in [
            'output0', 'output1', 'output2']])

        for i, confidence in enumerate(confidences):
            if confidence < self.threshold:
                print('{} {} found'.format(i, self.detection_type))
                return True, \
                    bounding_boxes[:i] * np.array([height_mul, width_mul, height_mul, width_mul]), \
                    classes[:i], \
                    confidences[:i]
        return True, bounding_boxes, classes, confidences

    # Load object labels from file
    def read_object_list(self, object_list_path):
        self.object_list = None
        self.detection_type = 'Objects'
        if object_list_path is not None:
            object_list_file = open(object_list_path, 'r')
            self.object_list = object_list_file.read().splitlines()
            self.detection_type = self.object_list[0]

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
                            self.object_list[int(np.asscalar(obj_class)) + 1],
                            (int(bounding_box[1] * width), int(bounding_box[0] * height - 10)),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            1,
                            color,
                            2)
                print(self.object_list[int(np.asscalar(obj_class)) + 1])
        return image

    # Read environment variables
    def read_enviroment(self):
        self.threshold = float(os.environ.get('DETECTION_THRESHOLD', 0.5))
        self.inference_client = InferenceClient(os.environ['INFERENCE_HOST'], int(os.environ['INFERENCE_PORT']))
        self.model_name = os.environ['MODEL_NAME']
        self.model_input_width, self.model_input_height = [int(i) for i in os.environ['MODEL_INPUT_SIZE'].split('x')]
        image_path = os.environ.get('IMAGE_PATH')
        object_list_path = os.environ.get('OBJECT_LIST_PATH')
        return image_path, object_list_path

    # Run object detection
    def run(self):
        print(f"inference-client connect to: {os.environ['INFERENCE_HOST']}:{int(os.environ['INFERENCE_PORT'])}")
        image_path, object_list_path = self.read_enviroment()
        self.read_object_list(object_list_path)
        if image_path is not None:
            for i in range(5):
                self.run_image_source(image_path)
        else:
            self.run_camera_source()

    # Run object detection on video stream
    def run_camera_source(self):
        cap = cv2.VideoCapture(1)
        cap.set(cv2.CAP_PROP_FPS, 30)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 360)
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


# Rescale image to desired size
def rescale(image, width, height, maintain_ratio=True):
    if not maintain_ratio:
        return cv2.resize(image, (width, height))
    org_height, org_width = image.shape[:2]
    width_ratio = float(width) / org_width
    height_ratio = float(height) / org_height
    if width_ratio < height_ratio:
        tmp_width = width
        tmp_height = int(org_height * width_ratio)
        bottom, right = height - tmp_height, 0
        width_mul, height_mul = 1, float(height) / tmp_height
    elif width_ratio > height_ratio:
        tmp_width = int(org_width * height_ratio)
        tmp_height = height
        bottom, right = 0, width - tmp_width
        width_mul, height_mul = float(width) / tmp_width, 1
    else:
        return cv2.resize(image, (width, height)), 1, 1
    image = cv2.resize(image, (tmp_width, tmp_height))
    return cv2.copyMakeBorder(image, 0, bottom, 0, right, cv2.BORDER_CONSTANT, value=(0, 0, 0)), width_mul, height_mul


# Main program
if __name__ == '__main__':
    Detector().run()
