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

