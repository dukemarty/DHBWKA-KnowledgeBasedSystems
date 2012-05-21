/*! \if exist This is -*- C++ -*- from nbg \endif

    \file  Instance.cc

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

/* system includes */
#include <assert.h>

/* my includes */
#include "Instance.h"

NBL::Instance::Instance()
{
}

NBL::Instance::~Instance()
{
}

string NBL::Instance::toString() const
{
  string res = m_rootValue;
  res += " <- < ";
  for (vector<string>::const_iterator it=m_attribValues.begin(); it!=m_attribValues.end(); it++){
    res += *it + " ";
  }
  res += ">";

  return res;
}

