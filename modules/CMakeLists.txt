# Copyright: (C) 2014 RobotCub Consortium
# Author: ---
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

add_subdirectory(affManager)
add_subdirectory(objectFinder) # No need to run in simulation
# add_subdirectory(toolBlobber)  # No need to run in simulation

# 2D Specific module
# add_subdirectory(feat2DExt)	

# 3D specific modules
add_subdirectory(tool3DManager)

add_subdirectory(simtoolloader) #Use only in simulation

# Not fully developed modules
## add_subdirectory(affLearn)
## add_subdirectory(affStateMachine)
