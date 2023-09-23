/**
 * @file vmb-index-sync.cpp
 * @author Vyacheslav Anzhiganov (vanzhiganov@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2023
 * 
 * Downloads the image-index file https://dl.platform-os.org/images/image-index
 * to local cache dir.
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <stdlib.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/program_options.hpp>

#include "vmb.hpp"

using namespace std;
namespace po = boost::program_options;
namespace vmb = vmb;


int main(int argc, char** argv) {
    string version = "1.0";
    string config = "/etc/pos/config.ini";
    string cache_dir = "/var/lib/pos/images";
    string image_url = "https://dl.platform-os.org/images";

    po::options_description desc ("Allowed options");
    desc.add_options ()
        ("help,h", "print usage message")
        ("config,c", po::value<string>(), "config file path")
        ("version,v", "show version")
    ;

    // Parse command line arguments
    po::variables_map arg;
    po::store(po::parse_command_line(argc, argv, desc), arg);
    po::notify (arg);

    // Check if there are enough args or if --help is given
    if (arg.count("help")) {
        std::cerr << desc << endl;
        return 1;
    }
    if (arg.count("version")) {
        std::cout << version << endl;
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
    image_url = pt.get<std::string>("images.url");

    string url = image_url + "/image-index";

    // run download the images-index file
    // TODO: add various arches
    char * tmp = vmb::str2char(cache_dir + "/.image-index.download");
    char * dst = vmb::str2char(cache_dir + "/image-index");

    cout << "download start: " << url << endl;

    int dl_result = vmb::dl(vmb::str2char(url), tmp);

    if (dl_result == 0) {
        if (rename(tmp, dst) != 0) {
            perror("Error moving file");
        } else {

        }
    }

    cout << "finish" << endl;

    // // read ini file
    // boost::property_tree::ptree pt;
    // boost::property_tree::ini_parser::read_ini("images-index.ini", pt);

    // // pt

    // // string image_url = pt.get<std::string>("images.url");
    // // string cache_dir = pt.get<std::string>("images.cache_dir");

    // for (auto& section : pt) {
    //     std::cout << "[" << section.first << "]\n";
    //     for (auto& key : section.second) {
    //         std::cout << key.first << "=" << key.second.get_value("") << "\n";
    //     }
    // }

    return 0;
}