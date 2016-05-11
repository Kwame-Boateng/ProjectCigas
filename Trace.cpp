/************************************************************
* File name: Trace.cpp
* Authours: John Cigas & Kwame Boateng
*
* Program Discription: This program searches through a
* capture file (pcap) to replace a list of strings.
************************************************************/

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

/*
FUNCTION DISCRIPTION:
--This function checks a sequence of characters and compare it to another string
ARGUMENTS:
--int currpos = the position of the current character being checked.
--char sea[] = The string of characters to check for.
--string sub = The string of characters to be compared with char[] sea.
--int numOfMatch = Checks the number of character in sea[] equal to sub.
if the number of characters equal is the same as the lenght of the two
strings, then they match.
RETURN VALUE:
--This function does not return anything. It compares two strings if
they are equal.
*/
bool check_seq (int currpos, char sea[], string &sub, int numOfMatch = 0 )
{
    for (int i = 0; i < sub.length(); i++)
    {
        if (sub[i] == sea[currpos])
        {
            numOfMatch ++;
        }
        currpos++;
    }
    if (numOfMatch == sub.length())
    {
        return true;
    }
}

/*
FUNCTION DESCRIPTION:
--This function is used to replace a string with another string.
ARGUMENTS:
--char sea[] = This is the main array of characters the you want to search
and replace a string from.
--string &sub = The string that is to be replaced.
--string &rep = The string that replaces another string.
--int searchlen = The size of array of characters (sea[]) to perform
operation on.
RETURN VALUE:
--This function does not return anything. It replaces a list of
strings in an array of characters.
*/
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
    //a map of strings and keys which are to be replaced.
    map <string, string> search_chars;
    {
    search_chars["AnnsEmail"] = "sneakyg33k@aol.com";
    search_chars["AnnsEmailPassword"] = "NTU4cjAwbHo=";
    search_chars["AnnsLoversEmail"] = "mistersecretx@aol.com";
    search_chars["Attachment"] = "secretrendezvous.docx";
    search_chars["AnnsGreeting"] = "Hi sweetheart!";
    search_chars["Item1"] = "fake passport";
    search_chars["Item2"] = "bathing suit";
    search_chars["Answer2"] = "AppleTV/2.4";
    search_chars["Answer5"] = "http://a227.v.phobos.apple.com/us/r1000/008/Video/62/bd/1b/mzm.plqacyqb..640×278.h264lc.d2.p.m4v";
    search_chars["Answer6"] = "Sneakers";
    search_chars["Answer7"] = "$9.99";
    search_chars["Answer8"] = "iknowyourewatchingme";
    }

    bool debug = false; //Boolean to specify the state of debug flag.
    string Replacefile; //String to specify name of to read replacement strings from.
    string Searchfile("evidence02.pcap"); //String to specify the main file to perform operations on.
    vector <string> replace_chars; // Vector of replacement strings.
    string repstring; //Strings to read the lines from replacement file.

    if (argc > 1)
    {
        //This for loop checks for the flags specified on the command line.
        for (int i = 0; i < argc; i++)
        {
            string arg = argv[i];
            if (arg == "-f" && i != (argc-1))
            {
                Replacefile = argv[i + 1];
                ifstream Replace_File (Replacefile);
                if (Replace_File.is_open())
                {
                    while (getline(Replace_File, repstring))
                    {
                        replace_chars.push_back(repstring);
                    }
                }
                else
                {
                    cout << "could not open " << Replacefile << endl;
                }
                Replace_File.close();
            }
            else if (arg == "-r" && i != (argc-1))
            {
                   replace_chars.push_back(argv[i+1]);
            }
            else if (arg == "-s" && i != (argc-1))
            {
                Searchfile = argv[i+1];
            }
            else if (arg == "-h")
            {
                string printLine;
                ifstream helpFile ("Help.txt");
                cout << "Usage:" <<endl;
                cout << "more [options] " <<argv[0] << endl << endl;

                while (getline(helpFile, printLine))
                {
                    cout << printLine << endl;
                }
                helpFile.close();
                return 1;
            }
            else if (arg == "-d")
            {
                debug = true;
            }

        }
    }
    else
    {
        Replacefile = "Replace.txt";
                ifstream Replace_File (Replacefile);
                if (Replace_File.is_open())
                {
                    while (getline(Replace_File, repstring))
                    {
                        replace_chars.push_back(repstring);
                    }
                }
                else
                {
                    cout << "could not open " << Replacefile << endl;
                }
                Replace_File.close();

    }

    cout << "\nCharacters to be replaced"<<endl;
    for (int i = 0; i < replace_chars.size(); i++)
    {
        cout << replace_chars[i]<<endl;
    }


    streampos size_b;
    char * buf_location;

    ifstream traceFile;
    traceFile.open (Searchfile, ios::binary | ios::ate);
    if (traceFile.is_open())
    {
        size_b = traceFile.tellg();

        cout << endl;

        buf_location = new char[size_b];
        traceFile.seekg(0, ios::beg);
        traceFile.read (buf_location, size_b);
    }
    else
    {
        cout << "Failure to open file!!!" << endl;
    }

    traceFile.close();


    for (int index = 0; index < replace_chars.size(); index++)
    {
        int position = replace_chars[index].find(':');
        string key1 = replace_chars[index].substr(0, position);
        string key2 = replace_chars[index].substr (position+1);
        for (map <string, string>::iterator it = search_chars.begin(); it != search_chars.end(); ++it)
        {
            if (key1 == it->first)
            {
                replace (buf_location, it->second, key2, size_b);
                break;
            }
        }
    }


    ofstream New_traceFile ("Output File.txt", ios::binary | ios::ate | ios::trunc);
    if (New_traceFile.is_open())
    {
        New_traceFile.seekp(0, ios::beg);
        New_traceFile.write (buf_location, size_b);
        New_traceFile.seekp(0, ios::end);

        if (New_traceFile.tellp() != 0)
        {
           cout << endl << "File altered and transfered successfully" << endl;

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
