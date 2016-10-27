# Copyright: (C) 2015-2016 iCub-Facility - Istituto Italiano di Tecnologia
# Author: Tanis Mar
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

add_subdirectory(affCollector)
add_subdirectory(aff-explorer)


# 2D Specific module
add_subdirectory(affManager)
#add_subdirectory(feat2DExt)	

# 3D specific modules
add_subdirectory(tool3DManager)
add_subdirectory(drop-away)
add_subdirectory(affMotor)
add_subdirectory(simtoolloader) #Use only in simulation
#add_subdirectory(positionFilter) # No need to run in simulation

### Not fully developed or obsolete modules

#add_subdirectory(objectFinder) # No need to run in simulation
# add_subdirectory(toolBlobber)  # No need to run in simulation

# add_subdirectory(affLearn)
# add_subdirectory(affStateMachine)
