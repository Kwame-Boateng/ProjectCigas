#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

//This function compares different sequence of characters.
bool check_seq (int currpos, char sea[], string &sub, int cond = 0 )
{
    for (int i = 0; i < sub.length(); i++)
    {
        if (sub[i] == sea[currpos])
        {
            cond ++;
        }
        currpos++;
    }
    {
    if (cond == sub.length())
        return true;
    }
}//check_seq

//This function replaces a set of strings in character array.
void replace (char sea[], string &sub, string &rep, int searchlen)
{
for (int i = 0; i < searchlen; i++)
  {
      int j = 0;
      if (sea[i] == sub[j] && check_seq(i, sea, sub))
      {
                cout << sub << " is replaced with " << rep << " at position " << i << endl <<endl;
                for (int k = i; k < (i+rep.length()); k++)
                {
                    sea[k] = rep[j];
                    j++;
                }

      }

  }

}


int main (int argc, char* argv[])
{
    map <string, string> search_chars;
    search_chars["AnnsEmail"] = "sneakyg33k@aol.com";
    search_chars["AnnsEmailPassword"] = "NTU4cjAwbHo=";
    search_chars["AnnsLoversEmail"] = "mistersecretx@aol.com";
    search_chars["Attachment"] = "secretrendezvous.docx";
    search_chars["AnnsGreeting"] = "Hi sweetheart!";
    search_chars["Item 1"] = "fake passport";
    search_chars["Item 2"] = "bathing suit";
    search_chars["Answer2"] = "AppleTV/2.4";
    search_chars["Answer5"] = "http://a227.v.phobos.apple.com/us/r1000/008/Video/62/bd/1b/mzm.plqacyqb..640×278.h264lc.d2.p.m4v";
    search_chars["Answer6"] = "Sneakers";
    search_chars["Answer7"] = "$9.99";
    search_chars["Answer8"] = "iknowyourewatchingme";

    string fileName;
    vector <string> replace_chars;

    if (argc > 1)
    {
        for (int i = 0; i < argc; i++)
        {
            string repstring;
            string arg = argv[i];
            if (arg == "-f" && i != (argc-1))
            {
                fileName = argv[i + 1];
                ifstream Replace_File (fileName);
                if (Replace_File.is_open())
                {
                    while (getline(Replace_File, repstring))
                    {
                        replace_chars.push_back(repstring);
                    }
                }
                else
                {
                    cout << "could not open " << fileName << endl;
                }
                Replace_File.close(); break;
            }
            else if (arg == "-r" && i != (argc-1))
            {
               for (int index = (i+1); index < argc; index++)
               {
                   replace_chars.push_back(argv[index]);
               } break;
            }

        }
    }

    for (int i = 0; i < replace_chars.size(); i++)
    {
        cout << replace_chars[i]<<endl;
    }


    streampos size_b;
    char * buf_location;

    ifstream traceFile;
    traceFile.open ("evidence02.pcap", ios::binary | ios::ate);
    if (traceFile.is_open())
    {
        cout << "File open Successful" << endl;

        size_b = traceFile.tellg();

        cout << "Original file size = " << size_b << " bytes" << endl << endl;

        buf_location = new char[size_b];
        traceFile.seekg(0, ios::beg);
        traceFile.read (buf_location, size_b);
    }
    else
    {
        cout << "Failure to open file!!!" << endl;
    }

    traceFile.close();



    /*string file_line;
    string replace_chars;
    size_t pos;


    ifstream replacef (fileName);
    if (replacef.is_open())

    {
        for (map <string, string>::iterator it = search_chars.begin(); it != search_chars.end(); ++it)
        {
           if (!replacef.eof())
           {
               for (int i = 0; i < search_chars.size(); i++)
               {
                   getline (replacef, file_line);
                   pos = file_line.find (':') + 1;
                   replace_chars = file_line.substr (pos);
                 if (file_line.substr(0, it->first.length())== it->first)
                 {
                     replace (buf_location, it->second, replace_chars, size_b);
                     break;
                 }
               }

           }
           replacef.seekg(0, ios::beg);
        }
    }
    else{
        cout << "Failure to open file." << endl;
    }
    replacef.close();*/



    ofstream New_traceFile ("Output File.txt", ios::binary | ios::ate | ios::trunc);
    if (New_traceFile.is_open())
    {
        //cout << endl << "Output file size is now " << New_traceFile.tellp() << " bytes" <<endl;
        New_traceFile.seekp(0, ios::beg);
        New_traceFile.write (buf_location, size_b);
        New_traceFile.seekp(0, ios::end);

        if (New_traceFile.tellp() != 0)
        {
           cout << endl << "File altered and transfered successfully" << endl;
           cout << "Output file size: " << New_traceFile.tellp() << " bytes" <<endl;
        }


    }
    else
    {
        cout << "Failure to open file!!" << endl;
    }

    New_traceFile.close();

    ofstream New ("NewFile.pcap", ios::binary);
    if (New.is_open())
    {
        New.write(buf_location, size_b);
    }
    else
    {
        cout << "Unable to create a new PCAP file" << endl;
    }

    delete [] buf_location;
    New.close();
    return 0;
}
