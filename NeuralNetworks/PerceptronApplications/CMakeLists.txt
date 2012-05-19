# -*- cmake -*-
#
# Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
# Date of last change: 29.11.11
#
# Author: Martin Loesch (<loesch@@ira.uka.de>)
# Date: 2011-02-22
#
# Copyright: Martin Loesch, Chair Prof. Dillmann (HIS)
#            Institute for Anthropomatics (IFA)
#            Karlsruhe Institute of Technology (KIT). All rights reserved
#            http://his.anthropomatik.kit.edu


project(PerceptronApplications)

cmake_minimum_required( VERSION 2.8 )


# add_subdirectory( subproj )

include_directories(
  .
  /Users/martinloesch/Source/external/Flood3/Flood
  )

set( PerceptronApplicationsHeaders
  )
set( PerceptronApplicationsSources
  PerceptronApplications.cc
  )

set( PerceptronLearningHeaders
  PerceptronLearning.h
  )
set( PerceptronLearningSources
  PerceptronLearning.cc
  )

set( PerceptronLearningFWHeaders
  PerceptronLearningFramework.h
  )
set( PerceptronLearningFWSources
  PerceptronLearningFramework.cc
  )

set( PLClearHeaders
  PerceptronLearningFrameworkClear.h
  )
set( PLClearSources
  PerceptronLearningFrameworkClear.cpp
  )

set( FloodFiles
  /Users/martinloesch/Source/external/Flood3/Flood/Perceptron/Perceptron.cpp
  )

add_executable( PerceptronApplications ${PerceptronApplicationsHeaders} ${PerceptronApplicationsSources} ${FloodFiles} )
add_executable( PerceptronLearning ${PerceptronLearningHeaders} ${PerceptronLearningSources} ${FloodFiles} )
add_executable( PerceptronLearningFramework ${PerceptronLearningFWHeaders} ${PerceptronLearningFWSources} ${FloodFiles} )
add_executable( PLClear ${PLClearHeaders} ${PLClearSources} ${FloodFiles} )

file( COPY andData.txt
  DESTINATION . )

file( COPY orData.txt
  DESTINATION . )

file( COPY xorData.txt
  DESTINATION . )

file( COPY mystery.txt
  DESTINATION . )