"""
 * Copyright (C) 2020 Axis Communications AB, Lund, Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
"""

import cv2
import os
from vdo_proto_utils import VideoCaptureClient

# Setup VDO stream
grpc_socket = os.environ['INFERENCE_HOST']
stream_width, stream_height, stream_framerate = (640, 360, 10)
capture_client = VideoCaptureClient(socket=grpc_socket,
                                  stream_width=stream_width,
                                  stream_height=stream_height,
                                  stream_framerate=stream_framerate)

# Instatiate our detector
qr_detector = cv2.QRCodeDetector()

while True:
  # Read new frame
  frame = capture_client.get_frame()

  # Preprocessing
  # RGB to BGR to accommodate the QR detectors input signature
  frame = frame[:, :, ::-1]

  # Find and decode any QR codes present in image
  got_qr, text, _, _ = qr_detector.detectAndDecodeMulti(frame)

  # Output of results
  if got_qr:
    print('{pad}\n{text}\n{pad}'.format(pad='='*15, text=text))
  else:
    print('No QR found')

