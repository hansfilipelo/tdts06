#!/usr/bin/env python3

import sys
from LRUSim import *
from TTLSim import *

# -------------- main program
if __name__ == "__main__":

    programOption = int(sys.argv[1].replace("option=",""))

    if programOption == 1:
        minPriority = int(sys.argv[2].replace("X=",""))
        saveTime = int(sys.argv[3].replace("T=",""))
        outPutFile = sys.argv[4]
        TTLSimulator(minPriority,saveTime,outPutFile)
    elif programOption == 0:
        cacheMaxSize = int(sys.argv[2].replace("X=",""))
        minPriority = int(sys.argv[3].replace("Y=",""))
        outPutFile = sys.argv[4]
        LRUSimulator(cacheMaxSize,minPriority,outPutFile)
    else:
        print("Invalid program option")
