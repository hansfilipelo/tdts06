#!/usr/bin/env python3

import sys,os
import fileinput
import random
import collections

# ----------------
# cache[fileName] = {"hits" : 0, "size" : size, "priority" : priority, "incommingTime" : incommingTime, "duration" : videoDuration}

def TTLSimulator(minPriority, timeToSave, outPutFile):

    # ----------------
    hits = 0
    misses = 0

    cache = collections.OrderedDict()
    cacheSize = 0
    filesInCache = 0

    notInCache = dict()
    filesNeverInCache = 0
    bytesNeverInCache = 0

    limitBeforeEviction = 10 # Limit of statistics
    evictionMoreThanLimit = 0
    evictionLessThanLimit = 0

    # ----------------
    for line in sys.stdin:
        stdOutString = line.replace("\n","")
        currReq = line.split()
        incommingTime = float(currReq[0])
        videoDuration = int(currReq[2])
        fileName = currReq[4]
        priority = int(currReq[6])
        size = int(currReq[5])

        # Since dict is ordered we can clean from last reacently placed file
        # and work forward in cache - really simplifies implementation
        cacheKeys = list(cache.keys())
        i = 0
        while i < len(cacheKeys):
            currFile = cache[cacheKeys[i]]

            if int(currFile["incommingTime"]) + int(currFile["duration"]) + timeToSave < incommingTime:
                if int(currFile["hits"]) > limitBeforeEviction:
                    evictionMoreThanLimit += 1
                else:
                    evictionLessThanLimit += 1
                cacheSize -= currFile["size"]
                del cache[cacheKeys[i]]
                i += 1
                continue
            else:
                break # We can break here (and not go through entire list)
                # since dict keys is ordered

        # Don't save if not high priority
        if priority > minPriority:
            misses += 1
            if fileName in notInCache:
                pass
            else:
                notInCache[fileName] = {"hits" : 0, "size" : size, "priority" : priority, "incommingTime" : incommingTime, "duration" : videoDuration}
                filesNeverInCache += 1
                bytesNeverInCache += size
            stdOutString += "\t NOT_PUT_IN_CACHE \t Files_in_cache: " + str(filesInCache) + " \t Bytes_in_cache: " + str(cacheSize)
            print(stdOutString)
            continue

        # If in dict it's also in cache since we clean cache before hitting it
        if fileName in cache:
            hits += 1
            stdOutString += "\t ALREADY_CACHED \t Files_in_cache: " + str(filesInCache) + " \t Bytes_in_cache: " + str(cacheSize)
        # File not in cache
        else:
            misses += 1
            cacheSize += size
            filesInCache += 1
            cache[fileName] = {"hits" : 0, "size" : size, "priority" : priority, "incommingTime" : incommingTime, "duration" : videoDuration}
            stdOutString += "\t PUT_IN_CACHE \t Files_in_cache: " + str(filesInCache) + " \t Bytes_in_cache: " + str(cacheSize)
        print(stdOutString)

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
