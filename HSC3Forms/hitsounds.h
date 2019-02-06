#include <cstdio>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Volume
{
    string first;
    string first_green;
    string volume;
    string last;
    Volume()
    {
        volume = ""; first = ""; last = ""; first_green = "";
    }
    Volume(string d)
    {
        volume = d; first = ""; last = ""; first_green = "";
    }
};

struct Hitsound
{
    string HS;
    string Sampleset;
    string Additions;
    string Sample;
    Hitsound():HS("0"),Sampleset("0"),Additions("0"),Sample("0:0:")
    {
    }
};

struct Hitsoundm // osu!manĩa
{
    std::vector<string> HS;
    std::vector<string> Sampleset;
    std::vector<string> Additions;
    std::vector<string> Sample;
    Hitsoundm()
    {
    }
};

class Hitsounds
{
    std::map<int,double> MSPerUnit; // Timing
    std::map<int,Volume> Volumes;
    std::map<int,Hitsound> HS;
    std::map<int,Hitsoundm> HSm;
    std::vector<string> SbSamples;
public:
    bool CopyGreens;
    bool CopySamples;
    bool KeepBackup;
    Hitsounds();
    ~Hitsounds();
    string itos(int);
    void ReadFile(string);
    string Find(int,int);
    string FindV(int,int);
    string FindM(int,int,string * = NULL);
    void WriteFile(string f1, string f2);
};
