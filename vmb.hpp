#include <curl/curl.h>

#include <string>
#include <stdlib.h>
#include <iostream>
#include <filesystem>

using namespace std;

namespace vmb {
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
}