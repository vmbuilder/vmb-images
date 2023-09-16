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
#include <curl/curl.h>
#include <stdio.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <stdlib.h>

using namespace std;
namespace po = boost::program_options;
namespace fs = std::filesystem;


char * str2char(string str) {
    const int length = str.length();

    // declaring character array (+1 for null terminator)
    char* char_array = new char[length + 1];

    // copying the contents of the
    // string to char array
    strcpy(char_array, str.c_str());

    return char_array;
}

int dl(char * url, char * tmp_image) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();

    if (curl) {   
        fp = fopen(tmp_image, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "platform-os.org/agent v1.0");
        // todo: make only for verbose mode
        /* Switch on full protocol/debug output while testing */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        /* disable progress meter, set to 0L to enable it */
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}

std::vector<fs::path> remove_by_mask(fs::path const & root, std::string const & ext) {
    std::vector<fs::path> paths;

    if (fs::exists(root) && fs::is_directory(root)) {
        for (auto const & entry : fs::recursive_directory_iterator(root)) {
            if (fs::is_regular_file(entry) && entry.path().extension() == ext) {
                cout << entry << "\n";
                fs::remove(entry);
                // paths.emplace_back(entry.path().filename());
            }
        }
    }

    return paths;
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
        string image_name = arg["download"].as<string>();
        string url = image_url + "/" + image_name;

        char * tmp = str2char(cache_dir + "/." + image_name + ".download");
        char * dst = str2char(cache_dir + "/" + image_name);

        cout << "download start: " << url << "\n";

        int dl_result = dl(str2char(url), tmp);

        if (dl_result == 0) {
            if (rename(tmp, dst) != 0) {
                perror("Error moving file");
            } else {

            }
        }

        cout << "finish\n";
        return 0;
    }

    if (arg.count ("purge") && !arg.count("help") && !arg.count("version")) {
        cout << "purging: " << cache_dir << "\n";
        remove_by_mask(cache_dir, ".img");
        cout << "finish" << "\n";
        return 0;
    }

    cout << "nothing" << "\n";
    return 0;
}