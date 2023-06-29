#include <iostream>
#include <string>
#include<curl/curl.h>
using namespace std;

namespace Ezeksoft::SSHNotify
{
    class Request
    {
        public:
            static size_t callback(void *contents, size_t size, size_t nmemb, void *userp)
            {
                ((std::string *) userp)->append((char *) contents, size * nmemb);
                return size * nmemb;
            }

            string verb(string verb, string url)
            {
                CURL * curl;
                CURLcode res;
                std::string readBuffer;

                curl = curl_easy_init();
                if(curl)
                {
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, verb.c_str());
                    res = curl_easy_perform(curl);
                    curl_easy_cleanup(curl);
                }
                return readBuffer;
            }

            string get(string url)
            {
                return this->verb("GET", url);
            }
    };
}