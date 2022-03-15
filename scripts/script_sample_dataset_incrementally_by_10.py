#!/usr/bin/env python3
#requirements: opyenxes==0.3.0
from random import shuffle
from math import log10, trunc
from opyenxes.data_out.XesXmlSerializer import XesXmlSerializer
from opyenxes.factory.XFactory import XFactory
import os
import sys
from opyenxes.data_in.XUniversalParser import XUniversalParser
from opyenxes.model import XAttributeBoolean, XAttributeLiteral, XAttributeTimestamp, XAttributeDiscrete, XAttributeContinuous
from opyenxes.factory.XFactory import XFactory
from opyenxes.data_out.XesXmlSerializer import XesXmlSerializer

def pow_of_10(x):
	while (x >= 10) and (x % 10 == 0):
	  x /= 10
	return x == 1

if __name__ == '__main__':
   print(sys.argv[1])
   if (os.path.isfile(sys.argv[1])):
     print("ok")
     with open(sys.argv[1]) as file:
            log = XUniversalParser().parse(file)[0]
            
            logProps = log.get_attributes().clone()
            new_log = XFactory.create_log(logProps)
            new_log.get_extensions().update(log.get_extensions())
            new_log.__globalTraceAttributes = []
            new_log.__globalTraceAttributes.extend(log.get_global_trace_attributes())
            new_log.__globalEventAttributes = []
            new_log.__globalEventAttributes.extend(log.get_global_event_attributes())
            
            ll = len(log)
            print(ll)
            L = list(range(0,ll))
            shuffle(L)
            i = 0
            while True:
                new_log.append(log[L[i]])
                if (i>0) and (pow_of_10(i+1)):
                    with open(os.path.join(sys.argv[1][:-4] + "_" + str(i+1) + ".xes"), "w") as file:
                       XesXmlSerializer().serialize(new_log, file)
                       if (ll < i * 10): break
                i = i+1
