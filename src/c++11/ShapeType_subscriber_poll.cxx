/* ShapeType_subscriber.cxx

A subscription example

This file is derived from code automatically generated by the rtiddsgen 
command:

rtiddsgen -language C++11 -example <arch> ShapeType.idl

Example subscription of type rti::example::ShapeType automatically generated by 
'rtiddsgen'. To test them, follow these steps:

(1) Compile this file and the example publication.

(2) Start the subscription on the same domain used for RTI Data Distribution
Service  with the command
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

On UNIX systems: 

objs/<arch>/ShapeType_publisher <domain_id> 
objs/<arch>/ShapeType_subscriber <domain_id> 

On Windows systems:

objs\<arch>\ShapeType_publisher <domain_id>  
objs\<arch>\ShapeType_subscriber <domain_id>   

 */
#include <algorithm>
#include <iostream>

#include <dds/sub/ddssub.hpp>
#include <dds/core/ddscore.hpp>
// Or simply include <dds/dds.hpp> 

#include "ShapeType.hpp"

int subscriber_main(int domain_id, int sample_count)
{
	// Register types
	rti::domain::register_type<rti::example::ShapeType>("ShapeType");

	// Create a DomainParticipant from the named configuration
	dds::domain::DomainParticipant participant =
			dds::core::QosProvider::Default()->create_participant_from_config(
					"MyServiceIfLib::MyService.Sub");

	// Lookup DataReader
	dds::sub::DataReader<rti::example::ShapeType> reader =
			rti::sub::find_datareader_by_name<
			dds::sub::DataReader<rti::example::ShapeType> >(
					participant, "MySubscriber::MyReader");

	// Empty DataReader’s cache periodically
    for (int count = 0; count < sample_count || sample_count == 0; count++) {
		// Take all samples
		dds::sub::LoanedSamples<rti::example::ShapeType> samples = reader.take();
		for (auto sample : samples){
			if (sample.info().valid()){
				std::cout << sample.data() << std::endl;
			}
		}
		// The LoanedSamples destructor returns the loan

		std::cout << "rti::example::ShapeType subscriber sleeping..." << std::endl;
		rti::util::sleep(dds::core::Duration(4)); // poll every 4 seconds
	}
	return 1;
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
		subscriber_main(domain_id, sample_count);
	} catch (const std::exception& ex) {
		// This will catch DDS exceptions
		std::cerr << "Exception in subscriber_main(): " << ex.what() << std::endl;
		return -1;
	}

	// RTI Connext provides a finalize_participant_factory() method
	// if you want to release memory used by the participant factory singleton.
	// Uncomment the following line to release the singleton:
	//
	// dds::domain::DomainParticipant::finalize_participant_factory();

	return 0;
}
