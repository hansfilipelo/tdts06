#!/usr/bin/env python3

import sys
from LRUSim import *

# -------------- main program
if __name__ == "__main__":

    programOption = int(sys.argv[1].replace("option=",""))
    if programOption:
        print("Not implemented yet")
    else:
        cacheMaxSize = int(sys.argv[2].replace("X=",""))
        minPriority = int(sys.argv[3].replace("Y=",""))
        outPutFile = sys.argv[4]
        LRUSimulator(cacheMaxSize,minPriority,outPutFile)
