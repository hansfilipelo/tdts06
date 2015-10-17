#!/usr/bin/env python3

alpha = 0.125

sentTimes = [0.026477, 0.041737, 0.054026, 0.054690, 0.077405, 0.078157]
recvTimes = [0.053937, 0.077294, 0.124805, 0.169118, 0.217299, 0.267802]
SampleRTT = recvTimes[0]-sentTimes[0]
EstimatedRTT = SampleRTT

if len(sentTimes) == len(recvTimes):
    i = 0
    while i < len(sentTimes):
        EstimatedRTT = (1-alpha)*EstimatedRTT + alpha*SampleRTT
        print("Estimated for ", i,": ", EstimatedRTT)
        actual = recvTimes[i]-sentTimes[i]
        print("Actual RTT for ", i, ": ", actual)
        i += 1
