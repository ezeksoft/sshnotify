#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <nlohmann/json.hpp>
#include "helper.cpp"
#include "file.cpp"
#include "request.cpp"
#include "mailer.cpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    Ezeksoft::SSHNotify::File file;
    Ezeksoft::SSHNotify::Request request;
    Ezeksoft::SSHNotify::Mailer email;

    vector<string> e;
    bool readenv = false;
    string envpath;
   
    for (int i = 1; i < argc; i++)
    {
        string flag = argv[i];
        string value;
        
        if (argv[i + 1]) value = argv[i + 1];

        if (flag == "env")
        {
            readenv = true;
            if (value.length()) envpath = value;
        }
    }
    
    if (readenv && envpath.length()) e = file.read(envpath);

    vector<string> rows = file.read(env(e, "AUTHLOG_DB"));
    vector<string> sysrows = file.read(env(e, "AUTHLOG"));

    string search = "Accepted password for";
    string lastrow = array_ends_with(rows, search);
    string sys_lastrow = array_ends_with(sysrows, search);

    if (lastrow != sys_lastrow)
    {
        file.write(env(e, "AUTHLOG_DB"), join(sysrows, "\n"));

        vector<string> aux = split(sys_lastrow, ' ');

        string user = aux[8];
        string _ip = aux[10];
        string via = aux[13];
        string port = aux[12];
        string access_id = aux[4];
        string company = aux[3];
        string date = aux[0] + " " + aux[1] + " " + aux[2];
        
        string response = request.get("https://ipinfo.io/" + _ip);
        json data = json::parse(response);

        string ip = data.contains("ip") ? data["ip"] : "";
        string hostname = data.contains("hostname") ? data["hostname"] : "";
        string city = data.contains("city") ? data["city"] : "";
        string region = data.contains("region") ? data["region"] : "";
        string country = data.contains("country") ? data["country"] : "";
        string loc = data.contains("loc") ? data["loc"] : "";
        string org = data.contains("org") ? data["org"] : "";
        string postal = data.contains("postal") ? data["postal"] : "";
        string timezone = data.contains("timezone") ? data["timezone"] : "";
        string readme = data.contains("readme") ? data["readme"] : "";

        string subject = "Login as " + user + "!";
        string message = "Location: " + city + "/" + region + " - " + country + " Zipcode " + postal + "<br>" +
            "Timezone: " + timezone + "<br>" +
            "Coords: " + loc + "<br>" +
            "Reverse IP: " + hostname + "<br>" +
            "IP: " + ip + "<br>" +
            "Middle: " + via + " in port " + port + "<br>" +
            "Access ID: " + access_id + "<br>" +
            "Date: " + date + " (GMT-0)<br>" +
            "Extra: " + company + "<br>" +
            "<br>Raw: " + sys_lastrow
        ;

        string smtp_host = env(e, "SMTP_HOST");
        string smtp_username = env(e, "SMTP_USERNAME");
        string smtp_password = env(e, "SMTP_PASSWORD");
        string smtp_port = env(e, "SMTP_PORT");
        string smtp_from = env(e, "SMTP_FROM");
        string smtp_to = env(e, "SMTP_TO");
        string smtp_yourname = env(e, "SMTP_YOURNAME");

        vector<string> emaillist = split(smtp_to, ',');

        email.config(smtp_host, smtp_username, smtp_password, smtp_port);
        for (string _email : emaillist)
            email.send(smtp_from, _email, smtp_yourname, subject, message);
    }

    return 1;
}