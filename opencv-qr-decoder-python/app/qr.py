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
import numpy as np

# Setup VDO stream
cap = cv2.VideoCapture(0)

# Set resolution
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 360)

# Set image format
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('R', 'G', 'B', '3'))

# Set a small buffer size
cap.set(cv2.CAP_PROP_UNIMATRIX_MAX_BUFFERS, 1)

# Instatiate our detector
qr_detector = cv2.QRCodeDetector()

got_frame, _ = cap.read()
while got_frame:
  # Read new frame
  got_frame, frame = cap.read()

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

