# -*- cmake -*-
#
# Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
# Date of last change: 21.05.12
#
# Author: Martin Loesch (<loesch@@ira.uka.de>)
# Date: 2011-02-22
#
# Copyright: Martin Loesch, Chair Prof. Dillmann (HIS)
#            Institute for Anthropomatics (IFA)
#            Karlsruhe Institute of Technology (KIT). All rights reserved
#            http://his.anthropomatik.kit.edu


project(MlnnXorApplication)

cmake_minimum_required( VERSION 2.8 )

# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" )


find_package( Boost "1.37" REQUIRED COMPONENTS program_options thread )



include_directories(
  .
  /Users/martinloesch/Source/external/Flood3/Flood
  )

set( FloodSources
  /Users/martinloesch/Source/external/Flood3/Flood/MultilayerPerceptron/MultilayerPerceptron.cpp
  /Users/martinloesch/Source/external/Flood3/Flood/Perceptron/Perceptron.cpp
  /Users/martinloesch/Source/external/Flood3/Flood/ObjectiveFunctional/SumSquaredError.cpp
  /Users/martinloesch/Source/external/Flood3/Flood/ObjectiveFunctional/ObjectiveFunctional.cpp
  /Users/martinloesch/Source/external/Flood3/Flood/TrainingAlgorithm/TrainingAlgorithm.cpp
  /Users/martinloesch/Source/external/Flood3/Flood/TrainingAlgorithm/GradientDescent.cpp
  /Users/martinloesch/Source/external/Flood3/Flood/Utilities/InputTargetDataSet.cpp
)

set( SimpleMlnnApplicationsHeaders
  XorMlnn.h
  )

set( SimpleMlnnApplicationsSources
  MlnnFromFileApplication.cpp
  )

set( Ex3MlnnApplicationsHeaders
  Ex3Mlnn.h
  )

set( Ex3MlnnApplicationsSources
  MlnnEx3Application.cpp
  Ex3Mlnn.cpp
  )

add_executable( SimpleMlnnApplications ${SimpleMlnnApplicationsHeaders} ${SimpleMlnnApplicationsSources} ${FloodSources} )

add_executable( Ex3MlnnApplications ${Ex3MlnnApplicationsHeaders} ${Ex3MlnnApplicationsSources} ${FloodSources} )

#add_dependencies( PerceptronApplications
#
#  )

#target_link_libraries( PerceptronApplications
#
#  )

