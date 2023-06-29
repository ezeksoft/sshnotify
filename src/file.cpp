#include <vector>
#include <string>
#include <fstream>
using namespace std;

namespace Ezeksoft::SSHNotify
{
    class File
    {
        public: 
            vector<string> read(string path)
            {
                fstream file;
                vector<string> rows;
                file.open(path);

                if (file.is_open()) 
                {
                    string sysrow;
                    while(getline(file, sysrow))
                        rows.push_back(sysrow);  
                }
                return rows;
            }

        public:
            void write(string path, string content)
            {
                ofstream file(path);
                file << content;
                file.close();
            }
    };
}