import os
import numpy as np
from tf_proto_utils import InferenceClient
from vdo_proto_utils import VideoCaptureClient


grpc_socket = os.environ['INFERENCE_HOST']
stream_width, stream_height, stream_framerate = (224, 224, 10)
capture_client = VideoCaptureClient(socket=grpc_socket,
                                  stream_width=stream_width,
                                  stream_height=stream_height,
                                  stream_framerate=stream_framerate)
inf_client = InferenceClient(grpc_socket)

# The capture -> inference loop
while True:
    # Read an image from the video stream
    image = capture_client.get_frame()

    image = np.expand_dims(image, axis=0)

    # Perform inference with a model specified as an environment variable
    success, result = inf_client.infer({'data': image}, os.environ['MODEL_PATH'])

    # Do something with the result
    print(result)
