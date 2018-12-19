
/* ShapeType_subscriber.cxx

A subscription example

This file is derived from code automatically generated by the rtiddsgen 
command:

rtiddsgen -language C++ -example <arch> ShapeType.idl

Example subscription of type rti::example::ShapeType automatically generated by
'rtiddsgen'. To test them follow these steps:

(1) Compile this file and the example publication.

(2) Start the subscription with the command
objs/<arch>/ShapeType_subscriber <domain_id> <sample_count>

(3) Start the publication with the command
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

#include <stdio.h>
#include <stdlib.h>

#include "ShapeType.h"
#include "ShapeTypeSupport.h"
#include "ndds/ndds_cpp.h"

#include "ndds/ndds_namespace_cpp.h"

using namespace DDS;

namespace rti {
    namespace example {

        class ShapeTypeListener : public DataReaderListener {
          public:
            virtual void on_requested_deadline_missed(
                DataReader* /*reader*/,
                const RequestedDeadlineMissedStatus& /*status*/) {}

            virtual void on_requested_incompatible_qos(
                DataReader* /*reader*/,
                const RequestedIncompatibleQosStatus& /*status*/) {}

            virtual void on_sample_rejected(
                DataReader* /*reader*/,
                const SampleRejectedStatus& /*status*/) {}

            virtual void on_liveliness_changed(
                DataReader* /*reader*/,
                const LivelinessChangedStatus& /*status*/) {}

            virtual void on_sample_lost(
                DataReader* /*reader*/,
                const SampleLostStatus& /*status*/) {}

            virtual void on_subscription_matched(
                DataReader* /*reader*/,
                const SubscriptionMatchedStatus& /*status*/) {}

            virtual void on_data_available(DataReader* reader);
        };

        void ShapeTypeListener::on_data_available(DataReader* reader)
        {
            rti::example::ShapeTypeDataReader *ShapeType_reader = NULL;
            rti::example::ShapeTypeSeq data_seq;
            SampleInfoSeq info_seq;
            ReturnCode_t retcode;
            int i;

            ShapeType_reader = rti::example::ShapeTypeDataReader::narrow(reader);
            if (ShapeType_reader == NULL) {
                printf("DataReader narrow error\n");
                return;
            }

            retcode = ShapeType_reader->take(
                data_seq, info_seq, LENGTH_UNLIMITED,
                ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);

            if (retcode == RETCODE_NO_DATA) {
                return;
            } else if (retcode != RETCODE_OK) {
                printf("take error %d\n", retcode);
                return;
            }

            for (i = 0; i < data_seq.length(); ++i) {
                if (info_seq[i].valid_data) {
                    printf("Received data\n");
                    rti::example::ShapeTypeTypeSupport::print_data(&data_seq[i]);
                }
            }

            retcode = ShapeType_reader->return_loan(data_seq, info_seq);
            if (retcode != RETCODE_OK) {
                printf("return loan error %d\n", retcode);
            }
        }

    }
}

/* Delete all entities */
static int subscriber_shutdown(
    DomainParticipant *participant)
{
    ReturnCode_t retcode;
    int status = 0;

    if (participant != NULL) {
        retcode = participant->delete_contained_entities();
        if (retcode != RETCODE_OK) {
            printf("delete_contained_entities error %d\n", retcode);
            status = -1;
        }

        retcode = TheParticipantFactory->delete_participant(participant);
        if (retcode != RETCODE_OK) {
            printf("delete_participant error %d\n", retcode);
            status = -1;
        }
    }

    /* RTI Connext provides the finalize_instance() method on
    domain participant factory for people who want to release memory used
    by the participant factory. Uncomment the following block of code for
    clean destruction of the singleton. */
    /*

    retcode = DomainParticipantFactory::finalize_instance();
    if (retcode != RETCODE_OK) {
        printf("finalize_instance error %d\n", retcode);
        status = -1;
    }
    */
    return status;
}

extern "C" int subscriber_main(int domainId, int sample_count)
{
    DomainParticipant *participant = NULL;
    rti::example::ShapeTypeListener *reader_listener = NULL;
    rti::example::ShapeTypeDataReader *reader = NULL;
    ReturnCode_t retcode;
    int count = 0;
    Duration_t receive_period = {4,0};
    int status = 0;


    /* Register types */
    retcode = TheParticipantFactory->register_type_support(
    		rti::example::ShapeTypeTypeSupport::register_type,
			"ShapeType");
    if (retcode != RETCODE_OK) {
        printf("register_type error %d\n", retcode);
        subscriber_shutdown(NULL);
        return -1;
    }

    /* Create Participant from XML Config */
    participant = TheParticipantFactory->
            create_participant_from_config(
            		"MyServiceIfLib::MyService.Sub");
    if (participant == NULL) {
        printf("create_participant_from_config error\n");
        subscriber_shutdown(participant);
        return -1;
    }

    /* Lookup the DataReader */
    reader = rti::example::ShapeTypeDataReader::narrow(
    		participant->lookup_datareader_by_name(
    				"MySubscriber::MyReader"));
    if (reader == NULL) {
        printf("DataReader narrow error\n");
        subscriber_shutdown(participant);
        return -1;
    }


    /* Create a data reader listener */
    reader_listener = new rti::example::ShapeTypeListener();


    /* Main loop */
    for (count=0; (sample_count == 0) || (count < sample_count); ++count) {

        printf("rti::example::ShapeType subscriber sleeping for %d sec...\n",
        receive_period.sec);

        // poll the reader:
        reader_listener->on_data_available(reader);

        NDDSUtility::sleep(receive_period);
    }

    /* Delete all entities */
    status = subscriber_shutdown(participant);
    delete reader_listener;

    return status;
}

int main(int argc, char *argv[])
{
    int domainId = 0;
    int sample_count = 0; /* infinite loop */

    if (argc >= 2) {
        domainId = atoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = atoi(argv[2]);
    }

    /* Uncomment this to turn on additional logging
    NDDSConfigLogger::get_instance()->
    set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API, 
    NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */

    return subscriber_main(domainId, sample_count);
}

