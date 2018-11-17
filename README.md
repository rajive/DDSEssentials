# Overview

This example shows how to use DDS-XML Application Creation to define 
data-oriented interfaces and develop a modular distributed
system architecture. 
 
 The example shows how to use DDS-XML with
 
 - Compiled types with the Modern C++11 API
 - Dynamic types using the `rtiddsprototyper` (suitable for blackbox testing)

The data type used by this example is defined in the file 
[res/types/ShapeType.idl](res/types/ShapeType.idl), also 
used by the RTI Shapes Demo. 


The DDS-XML App Configuration is defined in the file [if/Shapes_if.xml](if/Shapes_if.xml). The `NDDS_QOS_PROFILES` environment variable defines the list of
XML files to be loaded automatically before a domain participant is created. 


# How to (re)build the example?

## Generate the types in XML (used by Lua: using dynamic types)

    rtiddsgen -ppDisable -convertToXml res/types/ShapeType.idl -d res/types
    
## Generate the types in C++11 (needed only for C++: using compiled types)

    rtiddsgen -ppDisable -language C++11 -unboundedSupport -stl -update typefiles res/types/ShapeType.idl -d src/c++11
    
    
## Setup makefile

- Determine your platform
   
  For, example on Mac OSX:
  
        export PLATFORM=x64Darwin17clang9.0 
    
- Generate makefile (needed only for C++)

	   rtiddsgen -ppDisable -language C++11 -unboundedSupport -stl -update makefiles -platform $PLATFORM res/types/ShapeType.idl -d src/c++11

- Create a soft-link to the generated makefile 
  (`makefile -> makefile_ShapeType_<platform>`)

        cd src/c++11
        ln -s makefile_ShapeType_$PLATFORM makefile
        cd ../..


## Build

From the top-level directory (containing this README):

    make


# How To run the example

Ensure that the environment variable `NDDS_QOS_PROFILES` list the DDS-XML files
to be loaded.

    export NDDS_QOS_PROFILES="res/qos/Shapes_qos.xml;if/Shapes_if.xml"

Run the components from the top-level directory (containing this README) 
as follows.

## Publisher

- c++11

	   ./src/c++11/objs/$PLATFORM/ShapeType_publisher 


- lua

	   rtiddsprototyper -cfgName MyServiceIfLib::MyService.Pub -luaFile src/lua/ShapeType_publisher.lua



## Subscriber

- c++11

        ./src/c++11/objs/$PLATFORM/ShapeType_subscriber 
    
- lua

	   rtiddsprototyper -cfgName MyServiceIfLib::MyService.Sub -luaFile src/lua/ShapeType_subscriber.lua

### Optional

- Run the *RTI Admin Console* to view the domain
- Run the *RTI Shapes Demo* to publish and subscribe to shapes 

