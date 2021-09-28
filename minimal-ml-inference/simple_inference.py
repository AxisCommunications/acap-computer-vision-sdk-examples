import os
import cv2
import numpy as np

from tf_proto_utils import InferenceClient

# Create the inference client with model server ip and port from environment variables
inf_client = InferenceClient(os.environ['INFERENCE_HOST'], int(os.environ['INFERENCE_PORT']))

# Create a video stream that fits the model inputs
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 300)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 300)
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*"RGB3"))

# The capture -> inference loop
while True:
    # Read an image from the video stream
    _, image = cap.read()

    # Give the image a batch dimension such that it has the shape (1, H, W, C)
    image = np.expand_dims(image, axis=0)

    # Perform inference with a model specified as an environment variable
    success, result = inf_client.infer({'data': image}, os.environ['MODEL_PATH'])

    # Do something with the result
    print(result)
