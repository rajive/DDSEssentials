# Overview

This example shows how to use DDS-XML Application Creation to define 
data-oriented interfaces and develop a modular distributed
system architecture. 
 
 The example shows how to use DDS-XML with
 
 - Compiled types with implementations in multiple languages
    - Modern C++ API (C++11)
    - Traditional C++ API (C++98)
 - Dynamic types using the `rtiddsprototyper` (suitable for blackbox testing)

The datatypes used by this example are defined in the file 
[res/types/ShapeType.idl](res/types/ShapeType.idl), and compatible with those 
used by the RTI Shapes Demo. 

The data-oriented inteterfaces using DDS-XML App Configuration are defined in 
the file [if/Shapes_if.xml](if/Shapes_if.xml). 

The `NDDS_QOS_PROFILES` environment variable specifies the list of XML files to 
be loaded automatically before a domain participant is created. 


# How to (re)build the example?

## Generate the types in XML (used by Lua: using dynamic types)

    rtiddsgen -ppDisable -convertToXml -d res/types res/types/ShapeType.idl
    
## Generate the C++ types (needed by C++ app using compiled types)

Modern C++ (C++11):

    rtiddsgen -ppDisable -language C++11 -unboundedSupport -stl -update typefiles -d src/c++11 res/types/ShapeType.idl 
    
    
Traditional C++ (C++98):

    rtiddsgen -ppDisable -language C++ -unboundedSupport -namespace -useStdString -update typefiles -d src/cpp res/types/ShapeType.idl 
    
    
    
## Setup makefile

- Determine your platform
   
  For, example on Mac OSX:
  
        export PLATFORM=x64Darwin17clang9.0 
    
- Generate makefile (needed for C++)

Modern C++ (C++11):

    rtiddsgen -ppDisable -language C++11 -unboundedSupport -stl -platform $PLATFORM -update makefiles -d src/c++11 res/types/ShapeType.idl 
	   
Traditional C++ (C++98):

    rtiddsgen -ppDisable -language C++ -unboundedSupport -namespace -useStdString -platform $PLATFORM -update makefiles -d src/cpp res/types/ShapeType.idl 
	   

- Create a soft-link to the generated makefile 
  (`makefile -> makefile_ShapeType_<platform>`)

        cd src/c++11  # OR    cd src/cpp
        ln -s makefile_ShapeType_$PLATFORM makefile
        cd ../..


## Build

From the top-level directory (containing this README):

    make


# How To run the example

Ensure that the environment variable `NDDS_QOS_PROFILES` list the DDS-XML files
to be loaded.

    export NDDS_QOS_PROFILES="res/qos/endpoint_qos.xml;res/qos/participant_qos.xml;res/qos/Shapes_qos.xml;if/Shapes.xml"

Run the components from the top-level directory (containing this README) 
as follows.

## Publisher

- c++11

	   ./src/c++11/objs/$PLATFORM/ShapeType_publisher

- c++98

          ./src/cpp/objs/$PLATFORM/ShapeType_publisher

- lua

	   rtiddsprototyper -cfgName ShapesIfLib::Shapes_Pub -luaFile src/lua/ShapeType_publisher.lua



## Subscriber

- c++11

          ./src/c++11/objs/$PLATFORM/ShapeType_subscriber

- c++98

          ./src/cpp/objs/$PLATFORM/ShapeType_subscriber

- lua

	   rtiddsprototyper -cfgName ShapesIfLib::Shapes_Sub -luaFile src/lua/ShapeType_subscriber.lua

### Optional

- Run the *RTI Admin Console* to view the domain
- Run the *RTI Shapes Demo* to publish and subscribe to shapes 

