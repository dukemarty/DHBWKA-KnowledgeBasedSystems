/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Instance.h
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

#ifndef INSTANCE_H
#define INSTANCE_H

/* system includes */
#include <string>
#include <vector>

/* my includes */
/* (none) */

using namespace std;


namespace NBL {

  /*!
    \class Instance
    \brief Representation of a data instance.
    
  */
  class Instance {
  private:
    
  protected:
    
  public:
    Instance();
    ~Instance();

    string m_rootValue;
    vector<string> m_attribValues;

    string toString() const;
  };

};

#endif /* INSTANCE_H */
