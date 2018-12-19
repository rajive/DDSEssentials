/* ShapeType_publisher.cxx

A publication of data of type ShapeType

This file is derived from code automatically generated by the rtiddsgen 
command:

rtiddsgen -language C++11 -example <arch> ShapeType.idl

Example publication of type ShapeType automatically generated by 
'rtiddsgen'. To test them follow these steps:

(1) Compile this file and the example subscription.

(2) Start the subscription on the same domain used for RTI Data Distribution
Service with the command
objs/<arch>/ShapeType_subscriber <domain_id> <sample_count>

(3) Start the publication on the same domain used for RTI Data Distribution
Service with the command
objs/<arch>/ShapeType_publisher <domain_id> <sample_count>

(4) [Optional] Specify the list of discovery initial peers and 
multicast receive addresses via an environment variable or a file 
(in the current working directory) called NDDS_DISCOVERY_PEERS. 

You can run any number of publishers and subscribers programs, and can 
add and remove them dynamically from the domain.

Example:

To run the example application on domain <domain_id>:

On Unix: 

objs/<arch>/ShapeType_publisher <domain_id> 
objs/<arch>/ShapeType_subscriber <domain_id> 

On Windows:

objs\<arch>\ShapeType_publisher <domain_id>  
objs\<arch>\ShapeType_subscriber <domain_id>   
*/

#include <iostream>

#include <dds/core/QosProvider.hpp>
#include <dds/pub/ddspub.hpp>
#include <rti/util/util.hpp> // for sleep()

#include "ShapeType.hpp"

void publisher_main(int domain_id, int sample_count)
{
	// Register types
	// Note: This demonstrates the use of DDS extensible types,
	//       since "PictureShapeType : ShapeType" in the IDL  definition.
	//       The PictureShapeType extends ShapeType and can contain a picture.
	//
	//       Both the IDL versions will be registered under the name "ShapeType"
	//       in the DDS domain, and the type assignability between the two
	//       structural representations is automatically handled by DDS.
	rti::domain::register_type<rti::example::PictureShapeType>("ShapeType");

    // Create a DomainParticipant from the named configuration
	dds::domain::DomainParticipant participant =
	        dds::core::QosProvider::Default()->create_participant_from_config(
	            "MyServiceIfLib::MyService.Pub");

    // Lookup DataWriter
    dds::pub::DataWriter<rti::example::PictureShapeType> writer =
			rti::pub::find_datawriter_by_name<
			dds::pub::DataWriter<rti::example::PictureShapeType> >(
				participant, "MyPublisher::MyWriter");

    rti::example::PictureShapeType sample;
	sample.color("RED");
    for (int count = 0; count < sample_count || sample_count == 0; count++) {
        // Modify the data to be written here
    	sample.x(20 + (count*10) % 230);
    	sample.y(100 + (count*10) % 230);
    	sample.shapesize(40);


    	// Large Data: un-comment the lines below to force a large sample
    	//    The picture size can exceed the UDP limit of 64KB, and illustrates
    	//    that the "large data" transfer scenario can be transparently
    	//    handled by DDS. The _qos.xml file configures the DataWriter's qos
    	//    policies to use asynchronous publishing in a background thread.
    	//
    	//sample.picture().dim(rti::picture::WIDTH);
		//sample.picture().pixel().resize(rti::picture::WIDTH * rti::picture::HEIGHT);

        std::cout << "Writing ShapeType, count " << count << std::endl;
        writer.write(sample);

        rti::util::sleep(dds::core::Duration(4));
    }
}

int main(int argc, char *argv[])
{

    int domain_id = 0;
    int sample_count = 0; // infinite loop

    if (argc >= 2) {
        domain_id = atoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = atoi(argv[2]);
    }

    // To turn on additional logging, include <rti/config/Logger.hpp> and
    // uncomment the following line:
    // rti::config::Logger::instance().verbosity(rti::config::Verbosity::STATUS_ALL);

    try {
        publisher_main(domain_id, sample_count);
    } catch (const std::exception& ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in publisher_main(): " << ex.what() << std::endl;
        return -1;
    }

    // RTI Connext provides a finalize_participant_factory() method
    // if you want to release memory used by the participant factory singleton.
    // Uncomment the following line to release the singleton:
    //
    // dds::domain::DomainParticipant::finalize_participant_factory();

    return 0;
}

