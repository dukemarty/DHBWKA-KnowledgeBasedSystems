/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Nodes.h
    \brief 

    Please put your documentation for this file here.

    \par Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
    \par Date of last change: 21.05.12

    \author   Martin Loesch (<loesch@@ira.uka.de>)
    \date     2010-04-08
    \par Copyright:
               Martin Loesch, Chair Prof. Dillmann (IAIM)\n
               Institute for Computer Science and Engineering (CSE)\n
	       University of Karlsruhe. All rights reserved\n
	       http://wwwiaim.ira.uka.de
*/

#ifndef NODES_H
#define NODES_H

/* system includes */
#include <string>
#include <map>

/* my includes */
/* (none) */

using namespace std;


namespace NBL {

  typedef map<unsigned int, string> TranslationTableIndex2Name;
  typedef map<string, unsigned int> TranslationTableName2Index;
  
  typedef enum {
    NONODETYPE,
    BOOLEAN,
    INTEGER,
    DISCRETE,
    CONTINUOUS
  } NodeValueType;

  bool stringToBool(string text);

  /*!
    \class Node
    \brief Abstract node class.

  */
  class Node {
  private:
    string _name;
    TranslationTableName2Index _translTableN2I;
    TranslationTableIndex2Name _translTableI2N;

  protected:
    NodeValueType m_type;
    unsigned int m_numberOfDiscreteValues;
    int m_intervalBegin, m_intervalEnd;

  public:
    Node(string name="");
    ~Node();
    
    void setName(string name);
    string getName() const;
    NodeValueType getType() const;
    unsigned int getNumberOfDiscreteValues() const;

    bool addDiscreteValue(string value);
    int translate(string value);
    string translate(int value);
    
    virtual void initBoolNode();
    virtual void initIntegerNode(int begin, int end);
    virtual void initDiscreteNode(unsigned int numberOfValues);
};
  
  /*!
    \class RootNode
    \brief Root node of a NB network.

    The main content is a list of probabilities for the diffferent states of the node.
  */
  class RootNode : public Node {
  private:
    float* _relProbabilities;
    
  protected:
    
  public:
    RootNode(string name="");
    ~RootNode();

    virtual void initBoolNode();
    virtual void initIntegerNode(int begin, int end);
    virtual void initDiscreteNode(unsigned int numberOfValues);

    void setProbabilities(float* probabilities, unsigned int length);
    float getProbability(int v);
  };

  /*!
    \class EndNode
    \brief End node of a NB network.

    Each end node is connected to a root node, and contains the conditional probabilities for its different states, depending on the root nodes state.
  */
  class EndNode : public Node {
  private:
    RootNode* _root;
    
    bool _boolValue;
    int _discreteValue;
    
    float** _condProbabilities;

  public:
    EndNode(RootNode* root, string name="");
    ~EndNode();

    virtual void initBoolNode();
    virtual void initIntegerNode(int begin, int end);
    virtual void initDiscreteNode(unsigned int numberOfValues);

    void setCondProbabilities(float* probabilities, unsigned int rows, unsigned int columns);
    float getCondProbForRootValue(int v);

    void setValue(bool value);
    void setValue(int value);
    void setValue(string value);
  };

};

#endif /* NODES_H */
