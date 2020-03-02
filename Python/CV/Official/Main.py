from readstreamlib import *
from urllib.request import Request, urlopen
import cv2, time

strm = stream("http://192.168.4.1")
startT = time.time()
while 1:
    readStatus = strm.readStream()
    if readStatus == "inComp": continue
    elif readStatus == "noImg": strm.disconHandle(); continue
    if strm.img.all() == None: print("ERROR, Stream Not Read")
    else: strm.basicSquareShapeDetection(1)
    if cv2.waitKey(1) == 27:
        endT = time.time()
        print("===== Stream Concluded =====")
        print("Length of Stream: {} seconds".format(endT-startT))
        print("Sent: {} frames | Lost: {} frames | Percentage Lost: {}".format(strm.frameNo, strm.lostFrame, strm.lostFrame/strm.frameNo))
        print("Average: {} Lost/minute".format((strm.lostFrame*60)/(endT-startT)))
        break