
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <curl/curl.h>

using namespace std;

namespace Ezeksoft::SSHNotify
{
    struct upload_status
    {
        size_t bytes_read;
    };

    static const char *payload_text;

    class Mailer
    {
        public:
            string host;
            string username;
            string password;
            string port;

            void config(string host, string username, string password, string port)
            {
                this->host = host;
                this->username = username;
                this->password = password;
                this->port = port;
            }

            static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
            {
                struct upload_status *upload_ctx = (struct upload_status *) userp;
                const char *data;
                size_t room = size * nmemb;

                if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1))
                    return 0;

                data = &payload_text[upload_ctx->bytes_read];

                if (data)
                {
                    size_t len = strlen(data);
                    if (room < len) len = room;
                    memcpy(ptr, data, len);
                    upload_ctx->bytes_read += len;
                    return len;
                }

                return 0;
            }

            int send(string from, string to, string yourname, string subject, string message)
            {
                ostringstream tmp ; tmp <<
                    "Content-Type: text/html; charset=utf-8\r\n" <<
                    "To: " << to << "\r\n" <<
                    "From: " << yourname << "<" << from << ">\r\n" <<
                    "Cc: " << from << "\r\n" <<
                    "Subject: " << subject << "\r\n" <<
                    "\r\n" <<
                    message <<
                    "\r\n"
                ;

                string str = tmp.str();
                payload_text = str.c_str();
                string url = string("smtp://") + this->host + string(":") + this->port;

                CURL *curl;
                CURLcode res = CURLE_OK;
                struct curl_slist *recipients = NULL;
                struct upload_status upload_ctx = { 0 };

                curl = curl_easy_init();
                if(curl)
                {
                    curl_easy_setopt(curl, CURLOPT_USERNAME, this->username.c_str());
                    curl_easy_setopt(curl, CURLOPT_PASSWORD, this->password.c_str());
                    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long) CURLUSESSL_ALL);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
                    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
                    recipients = curl_slist_append(recipients, to.c_str());
                    recipients = curl_slist_append(recipients, from.c_str());
                    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
                    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
                    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
                    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                    res = curl_easy_perform(curl);
                    if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    curl_slist_free_all(recipients);
                    curl_easy_cleanup(curl);
                }

                return (int) res;
            }
    };
}