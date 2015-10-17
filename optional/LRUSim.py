#!/usr/bin/env python3

# Output file format:

# the hit rate     what fraction of files was requested n times before cache eviction       what number of files and bytes was never stored on the cache.


# Each priority will get a key in a json dict.
# Each key value will contain another json dict containing the following keys:

# fileName(int)  hitsBeforeEviction(int)    size(int)   lastRequested(float)    duration(s)(int)

# ----------------
import sys,os
import fileinput
import random

# ----------------

def LRUSimulator(cacheMaxSize, minPriority, outPutFile):

    # ----------------
    hits = 0
    misses = 0

    cache = dict()
    cacheSize = 0
    filesInCache = 0

    filesNeverInCache = 0
    bytesNeverInCache = 0

    limitBeforeEviction = 10 # Limit of statistics
    evictionMoreThanLimit = 0
    evictionLessThanLimit = 0
    # ----------------

    for line in sys.stdin:
        currReq = line.split()
        incommingTime = float(currReq[0])
        videoDuration = int(currReq[2])
        fileName = currReq[4]
        priority = int(currReq[6])
        size = int(currReq[5])

        if priority > minPriority: # If priority of this file is higher (but lower ;) ) than the ones we cache, don't cache
            misses += 1
            filesNeverInCache += 1
            bytesNeverInCache += size
            continue

        if fileName in cache:
            hits += 1
            cache[fileName]["hits"] += int(cache[fileName]["hits"]) + 1
        else:
            misses += 1
            if cacheSize+size < cacheMaxSize:
                cache[fileName] = {"hits" : 0, "size" : size, "priority" : priority, "lastRequested" : incommingTime, "duration" : videoDuration}
                cacheSize += size
                filesInCache += 1
            else:
                loopCount = 0
                # While cache is full
                while cacheSize+size > cacheMaxSize:

                    # Don't try and fit the file in cache forever
                    if loopCount > filesInCache:
                        break
                    loopCount += 1

                    removeVideo = random.sample(cache.keys(),1)[0] # Pick any video
                    # If someone is watching this video, continue looking for other videos to throw away from cache
                    if float(cache[removeVideo]["lastRequested"]) + int(cache[removeVideo]["duration"]) > incommingTime:
                        continue

                    # Else remove element from cache
                    removedElement = cache.pop(removeVideo)
                    cacheSize -= int(removedElement["size"])
                    filesInCache -= 1
                    if int(removedElement["hits"]) > limitBeforeEviction:
                        evictionMoreThanLimit += 1
                    else:
                        evictionLessThanLimit += 1

                if loopCount > filesInCache:
                    continue
                else:
                    # When we've emptied cache enough, fit our new object in cache
                    cache[fileName] = {"hits" : 0, "size" : size, "priority" : priority, "lastRequested" : incommingTime, "duration" : videoDuration}
                    cacheSize += size
                    filesInCache += 1

    # ----------------
    # Output

    statString = "Hit percentage: " + str(hits/(hits+misses)) + "\n"

    if evictionMoreThanLimit+evictionLessThanLimit == 0:
        statString += "Fraction of files not evicted before " + str(limitBeforeEviction) + "hits : " + "No files evicted from cache. \n"
    else:
        statString += "Fraction of files not evicted before " + str(limitBeforeEviction) + " hits : " + str(evictionMoreThanLimit/(evictionMoreThanLimit+evictionLessThanLimit)) + "\n"

    statString += "Nr of files never in cache: " + str(filesNeverInCache) + "\n"

    os.system("cat /dev/null > " + outPutFile)
    writeFile = open(outPutFile,"a")
    writeFile.write(statString)

# ----------------
