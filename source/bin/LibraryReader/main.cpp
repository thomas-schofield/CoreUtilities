
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <memory>
#include <vector>
#include <string>
#include <chrono>

#include <iostream>
#include <fstream>
#include <cstdlib>

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

int main(int argc, char** argv)
{
    boost::program_options::variables_map opts{};
    parseOptions(opts, argc, argv);

    std::string test_file = "/Users/twscho/Developer/CoreUtilities/build/source/bin/BufferedStructure/BufferedStructure";

    std::ifstream reader(test_file, std::ios::in|std::ios::binary);
    if (!reader.is_open())
    {
        std::cerr << "File not open" << std::endl;
        return 0;
    }
    auto start = std::chrono::system_clock::now();

    reader.seekg(0, std::ios::end);
    std::size_t file_size = reader.tellg();
    reader.seekg(0, std::ios::beg);

    auto end = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Took ms to seek file: " << duration << std::endl;
    std::cout << file_size << std::endl;
    std::unique_ptr<char> buffer(new char[file_size]);
    reader.read(buffer.get(), file_size);
}