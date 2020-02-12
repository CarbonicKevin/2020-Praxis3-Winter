"""
Written on 20200127 by Kevin Kim
Basic 'square' detection software, not actual object detection, just looking for contors
To do:
Implement object shape recognition, and compare effectiveness
Implement code with camera stream, test effectiveness
    UPDATE: 20200127, Implemented reading stream, with help from https://github.com/espressif/arduino-esp32/issues/3597 and
"""

import cv2
from urllib.request import Request, urlopen
import numpy as np

def show(desc, img):
    scale_percent = 20 # percent of original size
    width = int(img.shape[1] * scale_percent / 100)
    height = int(img.shape[0] * scale_percent / 100)
    dim = (width, height)
    # resize image
    
    resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA) 
    cv2.imshow(desc, resized)

# Read From Stream
stream = urlopen('http://192.168.4.1')
bytes = b''
while True:
    bytes += stream.read(1024)
    a = bytes.find(b'\xff\xd8')
    b = bytes.find(b'\xff\xd9')

    if a != -1 and b != -1:
        jpg = bytes[a:b+2]
        bytes = bytes[b+2:]
        if jpg == b'': continue
        img = cv2.imdecode(np.fromstring(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)

        # Process Stream
        edges = cv2.Canny(img, 100, 200)
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
        dilate = cv2.dilate(edges, kernel, iterations=5)

        cnts = cv2.findContours(dilate, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = cnts[0] if len(cnts) == 2 else cnts[1]

        for c in cnts:
            area = cv2.contourArea(c)
            if area > 10000:
                x,y,w,h = cv2.boundingRect(c)
                if (h - 50 < w < h + 50) or (w - 50 < h < w + 50):
                    cv2.rectangle(img, (x, y), (x + w, y + h), (36,255,12), 2)

        # Display Stream
        cv2.imshow('a', img)
        cv2.imshow('b', edges)
        cv2.imshow('c', dilate)

        if cv2.waitKey(1) == 27:
            break