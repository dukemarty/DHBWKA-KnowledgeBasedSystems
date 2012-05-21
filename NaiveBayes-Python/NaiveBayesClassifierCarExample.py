#! /usr/bin/python
# This is -*- Python -*- from nbg -*- coding: latin1 -*-
#
##   \file  NaiveBayesClassifier.py
#    \brief Please put your documentation for this file here.
#
#    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
#    \par Date of last change: 15.12.11
#
#    \author   Martin Loesch (loesch@@ira.uka.de)
#    \date     2011-12-14
#    \par Copyright:
#              Martin Loesch, Chair Prof. Dillmann (HIS)\n
#              Institute for Anthropomatics (IFA)\n
#	       Karlsruhe Institute of Technology (KIT). All rights reserved\n
#	       http://his.anthropomatik.kit.edu
#

def normalizeDistribution(distribution):
    sum = 0.0
    for d in distribution:
        sum = sum + distribution[d]
    res = {}
    for d in distribution:
        res[d] = distribution[d] / sum
    return res


class Node:

    def __init__(self, name=""):
        self.name = name
        

class RootNode (Node):

    def __init__(self, name=""):
        Node.__init__(self, name)
        self.probabilites = {}
        self.distribution = { True:0.0, False:0.0 }

    def setName(self, name):
        self.name = name
        
    def setProbabilites(self, probs):
        self.probabilites = probs

    def __str__(self):
        res = "Root node  '" + self.name + "' -> " + str(self.distribution)
        res = res + "\n  " + str(self.probabilites)
        return res
        
        
class LeafNode (Node):

    def __init__(self, name=""):
        Node.__init__(self, name)
        self.evidence = False
        self.conditionalProbabilities = {}

    def setEvidence(self, belief):
        self.evidence = belief

    def setAllConditionalProbabilities(self, condProbs):
        self.conditionalProbabilities = condProbs

    def getCondProbForRootValue(self, rootValue):
        return self.conditionalProbabilities[self.evidence][rootValue]
        
    def __str__(self):
        res = "Leaf node '" + self.name + "' -> " + str(self.evidence)
        res = res + "\n  " + str(self.conditionalProbabilities)
        return res
        
        
class NaiveBayesClassifier:

    def __init__(self):
        self.root = RootNode()
        self.leaves = {}

    def setRootName(self, name):
        self.root.setName(name)

    def setRootProbabilityVector(self, probs):
        self.root.setProbabilites(probs)
        
    def addNode(self, name):
        if name in self.leaves:
            print "Error: node exists already!\n"
            return False
        else:
            self.leaves[name] = LeafNode(name)

    def setRootProbabilities(self, probs):
        self.root.setProbabilites(probs)
        
    def setConditionalNodeProbabilities(self, name, condProbs):
        self.leaves[name].setAllConditionalProbabilities(condProbs)

    def setLeafEvidence(self, name, belief):
        self.leaves[name].setEvidence(belief)

    def calculateRootProbabilities(self):
        trueProb = self.root.probabilites[True]
        for l in self.leaves:
            trueProb = trueProb * self.leaves[l].getCondProbForRootValue(True)
        self.root.distribution[True] = trueProb
        falseProb = self.root.probabilites[False]
        for l in self.leaves:
            falseProb = falseProb * self.leaves[l].getCondProbForRootValue(False)
        self.root.distribution[False] = falseProb

    def getRootDistribution(self):
        return self.root.distribution

    def getRootProbabilities(self):
        return normalizeDistribution(self.root.distribution)
            
    def __str__(self):
        res = str(self.root)
        res = res + "\n"
        for l in self.leaves:
            res = res + "\n" + str(self.leaves[l])
        return res
        
        
## TRUE MAIN PROGRAM
#
## \cond false
if __name__ == '__main__':
    print "*** Testing NaiveBayesClassifier.py ***\n"

    net = NaiveBayesClassifier()

    net.setRootName("Stolen?")
    rootProbs = { True:0.5, False:0.5}
    net.setRootProbabilities(rootProbs)

    net.addNode("Color=Red?")
    colorCondProbs = { True:{True:0.6, False:0.4}, False:{True:0.4, False:0.6} }
    net.setConditionalNodeProbabilities("Color=Red?", colorCondProbs)

    net.addNode("Type=SUV?")
    typeCondProbs = { True:{True:0.32, False:0.68}, False:{True:0.75, False:0.25} }
    net.setConditionalNodeProbabilities("Type=SUV?", typeCondProbs)
    
    net.addNode("Origin=Domestic?")
    foreignCondProbs = { True:{True:0.4, False:0.6}, False:{True:0.6, False:0.4} }
    net.setConditionalNodeProbabilities("Origin=Domestic?", foreignCondProbs)
        
    print "Results for <Color=Yellow> <Type=Sports> <Origin=Imported>"
    net.setLeafEvidence("Color=Red?", False)
    net.setLeafEvidence("Type=SUV?", False)
    net.setLeafEvidence("Origin=Domestic?", False)

    net.calculateRootProbabilities()
    print net.getRootDistribution()
    print net.getRootProbabilities()
    print
    
    print "Results for <Color=Red> <Type=SUV> <Origin=Domestic>"
    net.setLeafEvidence("Color=Red?", True)
    net.setLeafEvidence("Type=SUV?", True)
    net.setLeafEvidence("Origin=Domestic?", True)

    net.calculateRootProbabilities()
    print net.getRootDistribution()
    print net.getRootProbabilities()
    print
    
## \endcond
    
