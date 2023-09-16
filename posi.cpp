/**
 * @file posi.cpp
 * @author Vyacheslav Anzhiganov (vanzhiganov@ya.ru)
 * @brief 
 * @version 0.1
 * @date 2016-01-25
 * 
 * @copyright Copyright (c) Stack Web Services 2016
 * 
 */
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char** argv) {
    string version = "1.0";
    string config = "/etc/pos/config.ini";
    string cache_dir = "/var/lib/pos/images";
    string image;

    po::options_description desc ("Allowed options");
    desc.add_options ()
        ("help,h", "print usage message")
        ("config,c", po::value<string>(), "config file path")
        ("download,d", po::value<string>(), "download image. example: pos-image -d freebsd12.4")
        ("purge,p", "purge local cache")
        ("version,v", "show version")
    ;

    // Parse command line arguments
    po::variables_map arg;
    po::store(po::parse_command_line(argc, argv, desc), arg);
    po::notify (arg);

    // Check if there are enough args or if --help is given
    if (arg.count("help")) {
        std::cerr << desc << "\n";
        return 1;
    }
    if (arg.count("version")) {
        std::cout << version << "\n";
        
        return 0;
    }

    // Redefine config file path
    if (arg.count("config")) {
        config = arg["config"].as<string>();
    }

    // read ini file
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(config, pt);

    cache_dir = pt.get<std::string>("images.cache_dir");


    if (arg.count("download") && !arg.count("help") && !arg.count("version")) {
        cout << "download start\n";
        // TODO: download file and save into `config`
        cout << "finish\n";
        return 0;
    }

    if (arg.count ("purge") && !arg.count("help") && !arg.count("version")) {
        cout << "purging: " << cache_dir << "\n";
        // TODO: remove all files and folder from `cache_dir`
        return 0;
    }

    cout << "nothing" << "\n";
    return 0;
}