#!/usr/bin/env python3

# Input file format, tab separated:
# (i) the timestamp of the original request (decimal value), (ii) the client ID (string),
# (iii) the video duration (decimal value), (iv) the original server name (string), (v) the file name (string),
# (v) the total file size (decimal value), and (vi) a priority value between 0 and 10 (decimal value).

# timestamp     ClientID    Video duration      Original server     File name       Total filesize      priority

# ----------------

import random
import sys,os
import math

# -------------- main program
if __name__ == "__main__":
    argVector = sys.argv
    programName = argVector.pop(0)

    nrOfFiles = 100
    maxFileSize = 2 # This can't be bigger than specified cache size (X) : 36700160 bytes = 25 MiB
    maxDuration = 3600 # 3600s = 1h
    nrOfClients = 100
    requestRate = 0.005
    outPutFile = "input.txt"
    tempFile = "temp.txt"

    # Zipf distribution for popularity of the files (most files will be requested only a few times)
    # Prepare a constant according to: https://sv.wikipedia.org/wiki/Zipfs_lag
    cTemp = 0
    reallyLargeConstant = nrOfFiles*nrOfFiles
    for i in range(1,nrOfFiles):
        cTemp += i
    c = nrOfFiles*reallyLargeConstant/cTemp

    # Initialize a string thet we later write to file
    writeString = ""

    # For every file
    for i in range(1,nrOfFiles):
        priority = i
        size = random.randrange(1,maxFileSize,1)
        duration = random.randrange(1,maxDuration,1)
        server = "server" + str(i) + ".com"
        fileName = "file" + str(i) + ".mov"
        fileSize = random.randrange(1,maxFileSize,1)

        # Divide c with priority for number of requests for that specific file
        nrOfRequestsForFile = c/priority

        for it in range(1,int(round(nrOfRequestsForFile))):
            timeStamp = (1/requestRate)*(-math.log(random.random())) # Randomize incomming request time
            ClientID = random.randrange(1,nrOfClients,1)

            writeString += str(timeStamp) + "\t" + str(ClientID) + "\t" + str(duration) + "\t" + str(server) + "\t" + str(fileName) + "\t" + str(fileSize) + "\t" + str(priority) + "\n"

    # Write to file
    # ---------------

    os.system("cat /dev/null > " + outPutFile) # Clear file

    # Write to temp file
    writeFile = open(tempFile,"a")
    writeFile.write(writeString)
    # Sort the file on incomming times
    os.system("sort -k1 -n " + tempFile + " > " + outPutFile)
    os.system("rm " + tempFile)
