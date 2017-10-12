# Copyright: (C) 2015-2016 iCub-Facility - Istituto Italiano di Tecnologia
# Author: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

# 2D Specific module
add_subdirectory(affManager)
#add_subdirectory(feat2DExt)         # requires openCV
add_subdirectory(positionFilter)     # requires openCV

# 3D exploration modules
add_subdirectory(tool3DManager)
#add_subdirectory(aff-explorer-TCDS)

# demo specific modules
#add_subdirectory(xperience_sm)
#add_subdirectory(aff-explorer)
add_subdirectory(drop-away)
add_subdirectory(graspChecker)

# Generic modules
add_subdirectory(affMotor)
add_subdirectory(simtoolloader)     #Use only in simulation
add_subdirectory(affCollector)
