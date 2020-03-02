"""
Written by Kevin Kim 20200215
Small Function to take the given stream from urllib.request.urlopen, read and decode to a cv2 img. Returns image
"""
import cv2
from urllib.request import Request, urlopen
import numpy as np

class stream:
    def __init__(self, url):
        self.stream = urlopen(url)
        self.b = b''
        self.img = np.zeros(shape=[0, 0, 0], dtype=np.uint8)
        self.frameNo = 0
        self.lostFrame = 0

    def readStream(self):
        self.b += self.stream.read(1024)
        a = self.b.find(b'\xff\xd8')
        b = self.b.find(b'\xff\xd9')

        if a != -1 and b != -1:
            jpg = self.b[a:b+2]
            self.b = self.b[b+2:]
            if jpg == b'': return('noImg')
            self.img = cv2.imdecode(np.fromstring(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
            self.frameNo += 1
            return('Img')
        return('inComp')

    def disconHandle(self):
        print("Frame Loss Detected")
        n = 1
        while 1:
            readStatus = self.readStream()
            if   readStatus == "inComp": continue         # Image is not complete
            elif readStatus == "noImg" : continue; n += 1 # Lost Image
            elif readStatus == "Img"   : break            # Img Found
        self.lostFrame += n
        return()

    
    def basicSquareShapeDetection(self, debug=0):
        # Basic Shape Detection Algorithm
        # Uses Canny Algorithm to find edges, dilates edges, and then find countours

        edges = cv2.Canny(self.img, 100, 200)
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
        dilate = cv2.dilate(edges, kernel, iterations=5)

        cnts = cv2.findContours(dilate, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cnts = cnts[0] if len(cnts) == 2 else cnts[1]

        for c in cnts:
            area = cv2.contourArea(c)
            if area > 10000:  # Qualifier, size
                x,y,w,h = cv2.boundingRect(c)
                if (h - 50 < w < h + 50) or (w - 50 < h < w + 50):  # Qualifier, shape
                    cv2.rectangle(self.img, (x, y), (x + w, y + h), (36,255,12), 2)

        # Display Stream
        cv2.imshow('Processed Image', self.img)
        if debug == 1:
            cv2.imshow('Edges', edges)
            cv2.imshow('Dilate', dilate)