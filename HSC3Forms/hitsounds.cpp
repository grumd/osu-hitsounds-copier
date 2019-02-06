#include "hitsounds.h"
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>

Hitsounds::Hitsounds()
{
}

Hitsounds::~Hitsounds()
{
    MSPerUnit.clear();
    Volumes.clear();
    SbSamples.clear();
    HS.clear();
}

void Hitsounds::ReadFile(string f)
{
    std::fstream F;
    F.open(f.c_str(), std::fstream::in);

    string buf;
    string mode;
    char cbuf[4096];
    double SV = 0;
	char *token = NULL;
    SbSamples.clear();
    do
    {
        F.getline(cbuf,4096);
        buf = cbuf;
        if (buf.substr(0,17) == "SliderMultiplier:")
            SV = atof(buf.substr(17,buf.length()).c_str());
        if (buf.substr(0,6) == "Mode: ")
            mode = buf.substr(6,buf.length());
        if (buf.substr(0,7) == "Sample," && CopySamples)
            SbSamples.push_back(buf);
    }
    while (buf != "[TimingPoints]" && !F.eof());
	
    int time = 0;
    double ms = 0, redline = 0, amp = 0;
    string sample, custom, volume, last;
    MSPerUnit.clear();

    do
    {
        F.getline(cbuf,4096);
        buf = cbuf;
        if (buf == "[Colours]" || buf == "")
            break;
		
        time = atoi(strtok_s(cbuf,",",&token));
        ms = atof(strtok_s(NULL,",",&token));
        if (ms>0)
        {
            redline = ms;
            amp = 1;
        }
        else
        {
            amp = -ms/100.0;
        }
        MSPerUnit[time] = redline*amp/(SV*100);
        if (CopyGreens)
        {
            strtok_s(NULL,",",&token);
            sample = strtok_s(NULL,",",&token);
            custom = strtok_s(NULL,",",&token);
            volume = strtok_s(NULL,",",&token);
            if (last != sample + custom + volume)
                Volumes.insert(std::pair<int,Volume>(time,Volume(sample+","+custom+","+volume)));
            last = sample + custom + volume;
        }
    }
    while (!F.eof());

    do
    {
        F.getline(cbuf,4096);
        buf = cbuf;
    }
    while (buf != "[HitObjects]" && !F.eof());

    string basehs;
    int repeats = 0, spinnerend = 0, type = 0;
    double units = 0, msperunit = 0;
    char* result = NULL;

    do
    {
        F.getline(cbuf,4096);
        buf = cbuf;
        if (buf == "")
            continue;
		//printf(cbuf);
		//printf("\n");
        strtok_s(cbuf,",",&token);
        strtok_s(NULL,",",&token);
        time = atoi(strtok_s(NULL,",",&token));
        type = atoi(strtok_s(NULL,",",&token));
        if (mode == "3")
        { // osu!mania
            HSm[time].HS.push_back(strtok_s(NULL,",",&token));
            if (type >= 128) // Slider
                strtok_s(NULL,",:",&token);
            HSm[time].Sampleset.push_back(strtok_s(NULL,",:",&token));
            HSm[time].Additions.push_back(strtok_s(NULL,",:",&token));
            HSm[time].Sample.push_back(strtok_s(NULL,"",&token));
        }
        else
        {
            // Standard or ctb or taiko
            // 36,176,11173,1,14,0:0:0:0:
            // 116,236,14923,2,0,L|152:232,4,29.2499991073609,8|4|6|2|8,0:0|0:0|0:0|0:0|0:0,0:0:0:0:
            // 256,192,97179,12,0,98351,0:0:0:0:
            if (type & 1 == 1)
            { // Circle
                HS[time].HS = strtok_s(NULL,",",&token);
                HS[time].Sampleset = strtok_s(NULL,",:",&token);
                HS[time].Additions = strtok_s(NULL,",:",&token);
                HS[time].Sample = strtok_s(NULL,"",&token);
            }
            else
            { // Slider or spinner
                basehs = strtok_s(NULL,",",&token);
                spinnerend = atoi(strtok_s(NULL,",",&token));
                if (spinnerend != 0)
                { // Spinner
                    HS[spinnerend].HS = basehs;
                    HS[spinnerend].Sampleset = strtok_s(NULL,",:",&token);
                    HS[spinnerend].Additions = strtok_s(NULL,",:",&token);
                    HS[spinnerend].Sample = strtok_s(NULL,"",&token);
                }
                else
                { // Slider
                    repeats = atoi(strtok_s(NULL,",",&token));
                    units = atof(strtok_s(NULL,",",&token));
                    for (std::map<int,double>::iterator it=MSPerUnit.begin(); it!=MSPerUnit.end(); ++it)
                    {
                        if (it->first > time)
                            break;
                        msperunit = it->second;
                    }
                    for (int i=0;i<=repeats;i++)
                    {
                        result = strtok_s(NULL,"|,",&token);
                        if (result == NULL)
                            HS[time + ceil(i*units*msperunit)].HS = basehs;
                        else
                            HS[time + ceil(i*units*msperunit)].HS = result;
                    }
					for (int i=0;i<=repeats;i++)
                    {
                        if (result == NULL) break;
                        HS[time + ceil(i*units*msperunit)].Sampleset = strtok_s(NULL,":,|",&token);
                        HS[time + ceil(i*units*msperunit)].Additions = strtok_s(NULL,":,|",&token);
                    }
                    if (result != NULL)
                    {
                        strtok_s(NULL,":,",&token);
                        strtok_s(NULL,":,",&token);
                        HS[time].Sample = strtok_s(NULL,"",&token);
                    }
                }
            }
        }
    }
    while (!F.eof());
    F.close();
}


string Hitsounds::Find(int time, int type)
{
    for (int i=-1;i<=1;i++) // May be not even needed to search +-1 ms. Just in case made it
    if (HS.find(time+i)!=HS.end())
    {
        switch (type)
        {
        case 1: return HS[time+i].HS;
        case 2: return HS[time+i].Sampleset;
        case 3: return HS[time+i].Additions;
        case 4: return HS[time+i].Sample;
        default: return "";
        }
    }
    switch (type) // not found
    {
    case 1: return "0";
    case 2: return "0";
    case 3: return "0";
    case 4: return "0:0:";
    default: return "";
    }
}

string Hitsounds::FindV(int time, int type)
{
    string result = "1,0,100";
    for (std::map<int,Volume>::iterator it=Volumes.begin(); it!=Volumes.end(); ++it)
    {
        if (it->first > time)
            break;
        switch (type)
        {
        case 1: result = itos(it->first);
        case 2: result = it->second.volume;
        }
    }
    return result;
}

string Hitsounds::FindM(int time, int type, string *add1)
{   // This function is magic. Don't look at it.
    int n = 0;
    int max = 0;
    string add = "0";
    for (int i=-1;i<=1;i++)
    if (HSm.find(time+i)!=HSm.end())
    {
        switch (type)
        {
        case 1:
            if (HSm[time+i].HS.size() == 0)
                return "0";
            max = -1;
            for (int j=0;j<HSm[time+i].HS.size();j++)
                if (atoi(HSm[time+i].HS[j].c_str())>=n)
                {
                    n = atoi(HSm[time+i].HS[j].c_str());
                    max = j;
                }
            add1->clear();
            add = HSm[time+i].Additions[max];
            if (add == "0")
            {
                add1->append("s");
                add = HSm[time+i].Sampleset[max];
            }
            else
                add1->append("a");
            add1->append(add);
            for (int j=0;j<HSm[time+i].HS.size();j++)
            {
                if (HSm[time+i].Additions[j] == add)
                {
                    n |= atoi(HSm[time+i].HS[j].c_str());
                    HSm[time+i].HS.erase(HSm[time+i].HS.begin() + j);
                    HSm[time+i].Additions.erase(HSm[time+i].Additions.begin() + j);
                    //HSm[time+i].Sampleset.erase(HSm[time+i].Sampleset.begin() + j);
                }
                else
                    if (HSm[time+i].Sampleset[j] == add && HSm[time+i].Additions[j] == "0")
                    {
                        n |= atoi(HSm[time+i].HS[j].c_str());
                        HSm[time+i].HS.erase(HSm[time+i].HS.begin() + j);
                        HSm[time+i].Additions.erase(HSm[time+i].Additions.begin() + j);
                        HSm[time+i].Sampleset.erase(HSm[time+i].Sampleset.begin() + j);
                    }
            }
            return itos(n);
        case 2:
            if (HSm[time+i].Sampleset.size() == 0)
                return "0";
            n = atoi(HSm[time+i].Sampleset[0].c_str());
            HSm[time+i].Sampleset.erase(HSm[time+i].Sampleset.begin());
            return itos(n);
        case 3:
            if (HSm[time+i].Additions.size() == 0)
                return "0";
            n = atoi(HSm[time+i].Additions[0].c_str());
            HSm[time+i].Additions.erase(HSm[time+i].Additions.begin());
            return itos(n);
        case 4:
            if (HSm[time+i].Sample.size() == 0)
                return "0:0:";
            add = "0:0:";
            max = -1;
            for (int j=0;j<HSm[time+i].Sample.size();j++)
            {
                if (HSm[time+i].Sample[j].length()>=add.length())
                {
                    add = HSm[time+i].Sample[j];
                    max = j;
                }
            }
            if (max > -1)
                HSm[time+i].Sample.erase(HSm[time+i].Sample.begin() + max);
            return add;
        default:
            return "";
        }
    }
    switch (type)
    {
    case 1: return "0";
    case 2: return "0";
    case 3: return "0";
    case 4: return "0:0:";
    default: return "";
    }
}

string Hitsounds::itos(int value)
{
  std::ostringstream o;
  if (!(o << value))
      return "";
  return o.str();
}

void Hitsounds::WriteFile(string f1, string f2)
{
    std::fstream F1;
    std::fstream F2;
    F1.open(f1.c_str(), std::fstream::in);
    F2.open(f2.c_str(), std::fstream::out | std::fstream::trunc);

    string buf;
    string mode;
    string out;
    char cbuf[4096];
    double SV = 0;
    do
    {
        //fgets(cbuf,4096,F1);
        F1.getline(cbuf,4096);
        buf = cbuf;
        F2 << cbuf << endl;
        if (buf.substr(0,6) == "Mode: ")
            mode = buf.substr(6,buf.length());
        if (buf.substr(0,17) == "SliderMultiplier:")
            SV = atof(buf.substr(17,buf.length()).c_str());
        if (buf.substr() == "//Storyboard Sound Samples" && CopySamples)
            for (int i=0;i<SbSamples.size();i++)
                F2 << SbSamples[i] << endl;
    }
    while (buf != "[TimingPoints]" && !F1.eof());
    double ms = 0, redline = 0, amp = 0;
    int time = 0;
    string ms_s;
    string first;
    string last;
    string volume;
    MSPerUnit.clear();
	char *token = NULL;
    do
    {
        F1.getline(cbuf,4096);
        buf = cbuf;
        if (buf == "[Colours]" || buf == "" || buf == "[HitObjects]")
            break;
        if (!CopyGreens)
            F2 << cbuf << endl;
        time = atoi(strtok_s(cbuf,",",&token));
        ms_s = strtok_s(NULL,",",&token);
        ms = atof(ms_s.c_str());
        if (ms>0)
        {
            redline = ms;
            amp = 1;
        }
        else
        {
            amp = -ms/100.0;
        }
        MSPerUnit[time] = redline*amp/(SV*100);
        if (CopyGreens)
        {
            first = ms_s+",";
            first.append(strtok_s(NULL,",",&token));
            volume = strtok_s(NULL,",",&token);
            volume.append(",");
            volume.append(strtok_s(NULL,",",&token));
            volume.append(",");
            volume.append(strtok_s(NULL,",",&token));
            last = strtok_s(NULL,",",&token);
            last.append(",");
            last.append(strtok_s(NULL,",",&token));
            if (Volumes[time].first == "")
                Volumes[time].first = first;
            else
                Volumes[time].first_green = first;
            Volumes[time].last = last;
        }
    }
    while (!F1.eof());

    if (CopyGreens)
    {
        std::map<int,Volume>::iterator prev = Volumes.begin();
        for (std::map<int,Volume>::iterator it=Volumes.begin(); it!=Volumes.end(); ++it)
        {
            if (it->second.volume == "")
                it->second.volume = prev->second.volume;
            if (it->second.first == "" && it->second.last == "" && it!=Volumes.begin())
            {
                if (prev->second.first.substr(0,1) == "-")
                    it->second.first = prev->second.first;
                else
                    it->second.first = "-100," + prev->second.first.substr(prev->second.first.length()-1,1);
                it->second.last = prev->second.last;
            }
            else
                if (it->second.first == "" && it->second.last == "")
                    Volumes.erase(it);
            prev = it;

            if (it->second.first_green != "" && it->second.first.substr(0,1) != "-")
            {
                F2 << itos(it->first) << "," << it->second.first << "," << it->second.volume << ",1," << it->second.last.substr(it->second.last.length()-1,1) << endl;
                F2 << itos(it->first) << "," << it->second.first_green << "," << it->second.volume << ",0," << it->second.last.substr(it->second.last.length()-1,1) << endl;
            }
            else
                F2 << itos(it->first) << "," << it->second.first << "," << it->second.volume << "," << it->second.last << endl;
        }
    }
    F2 << cbuf << endl;
    while (buf != "[HitObjects]")
    {
        F1.getline(cbuf,4096);
        buf = cbuf;
        F2 << buf << endl;
    }
    string units_s;
    string basehs, slider;
    int repeats = 0, spinnerend = 0, type = 0;
    double units = 0, msperunit = 0;
    do
    {
        F1.getline(cbuf,4096);
        buf = cbuf;
        if (buf == "")
        {
            continue;
        }
        out.clear();
        out.append(strtok_s(cbuf,",",&token));
        out.append(",");
        out.append(strtok_s(NULL,",",&token));
        out.append(",");
        time = atoi(strtok_s(NULL,",",&token));
        out.append(itos(time));
        out.append(",");
        type = atoi(strtok_s(NULL,",",&token));
        out.append(itos(type));
        out.append(",");
        if (mode == "3")
        { // osu!mania
            string res;
            out.append(FindM(time,1,&res)+",");
            if (type >= 128) // Hold
            {
                strtok_s(NULL,",:",&token);
                out.append(strtok_s(NULL,",:",&token));
                out.append(":");
            }
            if (res.substr(0,1) == "s")
                out.append(res.substr(1,1)+":0:");
            else
            if (res.substr(0,1) == "a")
                out.append(FindM(time,2)+":"+res.substr(1,1)+":");
            else
                {
                    out.append(FindM(time,2)+":");
                    out.append(FindM(time,3)+":");
                }
            out.append(FindM(time,4)+"\n");
        }
        else
        // Standard or ctb or taiko
        if (type & 1 == 1)
        { // Circle
            // 36,176,11173,1,14,0:0:0:0:
            out.append(Find(time,1)+",");
            out.append(Find(time,2)+":");
            out.append(Find(time,3)+":");
            out.append(Find(time,4)+"\n");
        }
        else
        { // Slider or spinner
            basehs = strtok_s(NULL,",",&token);
            slider = strtok_s(NULL,",",&token);
            spinnerend = atoi(slider.c_str());
            if (spinnerend != 0)
            { // Spinner
                // 256,192,97179,12,0,98351,0:0:0:0:
                out.append(Find(spinnerend,1) + ",");
                out.append(itos(spinnerend));
                out.append(",");
                out.append(Find(spinnerend,2) + ":");
                out.append(Find(spinnerend,3) + ":");
                out.append(Find(spinnerend,4) + "\n");
            }
            else
            { // Slider
                // 116,236,14923,2,0,L|152:232,4,29.2499991073609,8|4|6|2|8,0:0|0:0|0:0|0:0|0:0,0:0:0:0:
                out.append(basehs+",");
                out.append(slider+",");
                repeats = atoi(strtok_s(NULL,",",&token));
                out.append(itos(repeats));
                out.append(",");
                units_s = strtok_s(NULL,",",&token);
                units = atof(units_s.c_str());
                out.append(units_s+",");
                for (std::map<int,double>::iterator it=MSPerUnit.begin(); it!=MSPerUnit.end(); ++it)
                {
                    if (it->first > time)
                        break;
                    msperunit = it->second;
                }
                for (int i=0;i<=repeats;i++)
                {
                    out.append(Find(time + ceil(i*units*msperunit),1));
                    if (i < repeats)
                        out.append("|");
                    else
                        out.append(",");
                }
                for (int i=0;i<=repeats;i++)
                {
                    out.append(Find(time + ceil(i*units*msperunit),2));
                    out.append(":");
                    out.append(Find(time + ceil(i*units*msperunit),3));
                    if (i < repeats)
                        out.append("|");
                    else
                        out.append(",0:0:");
                }
                out.append(Find(time,4)+"\n");
            }
        }
        F2 << out;
    }
    while (!F1.eof());
    F1.close();
    F2.close();
    if (!KeepBackup)
        remove(f1.c_str());
}
