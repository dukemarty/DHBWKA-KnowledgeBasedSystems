# -*- cmake -*-
#
# Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
# Date of last change: 21.05.12
#
# Author: Martin Loesch (<loesch@@ira.uka.de>)
# Date: 2012-05-21
#
# Copyright: Martin Loesch, Chair Prof. Dillmann (HIS)
#            Institute for Anthropomatics (IFA)
#            Karlsruhe Institute of Technology (KIT). All rights reserved
#            http://his.anthropomatik.kit.edu


project(NaiveBayesLibrary)

cmake_minimum_required( VERSION 2.8 )

# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" )


find_package( Boost "1.37" REQUIRED ) #  COMPONENTS program_options thread )


include_directories(
  .
  ${Boost_INCLUDE_DIRS}
  )

set( NaiveBayesLibraryHeaders
  Instance.h
  InstanceReader.h
  Nodes.h
  NaiveBayesNet.h
  Main.h
  )

set( NaiveBayesLibrarySources
  Instance.cc
  InstanceReader.cc
  Nodes.cc
  NaiveBayesNet.cc
  Main.cc
  )

add_executable( NaiveBayesLibrary ${NaiveBayesLibraryHeaders} ${NaiveBayesLibrarySources} )

#add_dependencies( NaiveBayesLibrary
#
#  )

#target_link_libraries( NaiveBayesLibrary
#
#  )

