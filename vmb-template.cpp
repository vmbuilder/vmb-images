/**
 * @file vmb-template.cpp
 * @author Vyacheslav Anzhiganov (vanzhiganov@ya.ru)
 * @brief 
 * @version 0.1
 * @date 2016-01-25
 * 
 * @copyright Copyright (c) Stack Web Services 2016
 * 
 */
// #include <curl/curl.h>
#include <stdio.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include "vmb.hpp"

using namespace std;
namespace po = boost::program_options;
namespace fs = std::filesystem;
namespace vmb = vmb;

std::vector<fs::path> remove_by_mask(fs::path const & root, std::string const & ext) {
    std::vector<fs::path> paths;
    if (fs::exists(root) && fs::is_directory(root)) {
        for (auto const & entry : fs::recursive_directory_iterator(root)) {
            if (fs::is_regular_file(entry) && entry.path().extension() == ext) {
                cout << entry << endl;
                fs::remove(entry);
            }
        }
    }
    return paths;
}

/**
 * @brief 
 * 
 * @param index_file 
 * @param distr 
 * @return std::pair<const std::string, boost::property_tree::ptree> 
 */
std::pair<const std::string, boost::property_tree::ptree> image_info(string index_file, string distr) {
    // read ini file
    boost::property_tree::ptree ii;
    boost::property_tree::ini_parser::read_ini(index_file, ii);
    for (auto& section : ii) {
        if (section.first == distr) {
            return section;
        }
    }
    // return;
}

int is_exists_image(boost::property_tree::ptree ii, string distr) {
    int is_exists = 0;
    for (auto& section : ii) {
        if (section.first == distr) {
            is_exists = 1;
            return is_exists;
        }
    }
    return is_exists;
}


int main(int argc, char** argv) {
    string version = "1.0";
    string config = "/etc/pos/config.ini";
    string cache_dir = "/var/lib/pos/images";
    string image_url = "https://dl.platform-os.org/images";

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

    image_url = pt.get<std::string>("images.url");
    cache_dir = pt.get<std::string>("images.cache_dir");

    if (arg.count("download") && !arg.count("help") && !arg.count("version")) {
        if (!std::filesystem::exists(cache_dir + "/image-index")) {
            perror("image-index file");
            exit(1);
        }
        string image_name = arg["download"].as<string>();
        string image_filename;
        string image_fileurl;

        // read ini file
        boost::property_tree::ptree ii;
        boost::property_tree::ini_parser::read_ini(cache_dir + "/image-index", ii);

        if (is_exists_image(ii, image_name) == 0) {
            cout << "no image in index file" << endl;
            exit(1);
        }

        image_filename = ii.get<std::string>(vmb::str2char(image_name + ".file"));
        image_fileurl = ii.get<std::string>(vmb::str2char(image_name + ".url"));
        
        if (std::filesystem::exists(cache_dir + "/" + image_filename)) {
            cout << "image already downloaded" << endl;
            exit(0);
        }

        char * tmp = vmb::str2char(cache_dir + "/." + image_filename + ".download");
        char * dst = vmb::str2char(cache_dir + "/" + image_filename);

        cout << "download start: " << image_fileurl << endl;

        if (vmb::dl(vmb::str2char(image_fileurl), tmp) == 0) {
            if (rename(tmp, dst) != 0) {
                perror("Error moving file");
            }
        }
        cout << "finish" << endl;

        return 0;
    }

    if (arg.count ("purge") && !arg.count("help") && !arg.count("version")) {
        cout << "purging: " << cache_dir << "\n";
        remove_by_mask(cache_dir, ".raw");
        remove_by_mask(cache_dir, ".qcow2");
        cout << "finish" << endl;
        return 0;
    }

    cout << "nothing" << endl;
    return 0;
}