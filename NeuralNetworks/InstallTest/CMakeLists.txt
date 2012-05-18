# -*- cmake -*-
#
# Last Author: Martin Loesch (<martin.loesch@@kit.edu>)
# Date of last change: 28.11.11
#
# Author: Martin Loesch (<loesch@@ira.uka.de>)
# Date: 2011-02-22
#
# Copyright: Martin Loesch, Chair Prof. Dillmann (HIS)
#            Institute for Anthropomatics (IFA)
#            Karlsruhe Institute of Technology (KIT). All rights reserved
#            http://his.anthropomatik.kit.edu


project(FloodInstallTest)

cmake_minimum_required( VERSION 2.8 )


# add_subdirectory( subproj )

include_directories(
  .
  /Users/martinloesch/Source/external/Flood3/Flood
  )

set( FloodInstallTestHeaders
  )

set( FloodInstallTestSources
  Main.cc
  )


set( FloodFiles

  /Users/martinloesch/Source/external/Flood3/Flood/Perceptron/Perceptron.cpp
  )

add_executable( FloodInstallTest ${FloodInstallTestHeaders} ${FloodInstallTestSources} ${FloodFiles} )

