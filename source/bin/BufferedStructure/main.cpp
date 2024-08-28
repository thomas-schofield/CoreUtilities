#include "IncrementalStructure.hpp"

#include "OverallStructure.hpp"

#include "copy_functions.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

#include <boost/program_options.hpp>

// Test that we can use boost in the project
void parseOptions(boost::program_options::variables_map& opts, int argc, char** argv)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message");

    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc),
        opts
    );
    boost::program_options::notify(opts);

    if (opts.count("help"))
    {
        std::cout << desc << "\n";
        exit(EXIT_SUCCESS);
    }
}

/*
 * We have a server app that publishes individual fields of OverallStructure.
 */
int main(int argc, char** argv)
{
    boost::program_options::variables_map opts{};
    parseOptions(opts, argc, argv);

    IncrementalStructure<int, OverallStructure> my_structure;

    MessageDataPod1 message1;
    message1.header.id = 1;
    message1.data_field_1 = 5;
    message1.data_field_2 = 2.0;

    MessageDataPod1 message2;
    message2.header.id = 2;
    message2.data_field_1 = 2;
    message2.data_field_2 = 2.5;

    MessageDataPod2 message3;
    message3.header.id = 1;
    message3.data_field_1 = 4.0;
    message3.data_field_2 = 5.0;

    MessageDataPod1 message4;
    message4.header.id = 1;
    message4.data_field_1 = 6;
    message4.data_field_2 = 10.0;

    // Perform setup of pod data
    my_structure.set<MessageDataPod1>(message1.header.id, message1, copy_data_pod_1);
    my_structure.set<MessageDataPod1>(message2.header.id, message2, copy_data_pod_1);
    my_structure.set<MessageDataPod2>(message3.header.id, message3, copy_data_pod_2);
    my_structure.set<MessageDataPod1>(message4.header.id, message4, copy_data_pod_1);


    OverallStructure entry1;
    OverallStructure entry2;
    bool result = my_structure.copy(1, entry1) && my_structure.copy(2, entry2);

    my_structure.remove(1);

    if (result)
    {
        entry1.print();
        entry2.print();
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "Failed to fill structures\n";
        return EXIT_FAILURE;
    }
}
