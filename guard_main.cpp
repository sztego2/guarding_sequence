#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

// exam times
int brownExamDay;
int brownExamTimeStart;
int brownExamTimeEnd;

int mokusoDay;
int mokusoTimeStart;
int mokusoTimeEnd;

int physicalExamDay;
int physicalExamTimeStart;
int physicalExamTimeEnd;

// no-underage night hours
int nightStartTime;
int nightEndTime;


struct nameAttr {
    string name;
    int rank;
    char masterOrStudent;
    bool isTakingExam;
    bool isTakingBrownExam;
    bool isUnderage;
    bool isTakingMokuso;
};


struct guard {
    int nameId;
    int hasBeenGuardCnt;
    int hasBeenNightGuardCnt;
};

struct guard_time {
    int day;
    int hour_time;
    string name1;
    string name2;
};

bool isAllowed (int nameId, unordered_map<int,nameAttr> names, int day, int hour, bool nightTime){
    nameAttr candidate = names[nameId];
    // underage at night shouldn't be guards
    if(nightTime && candidate.isUnderage) return false;
    // if it's mokuso time and the candidate takes it, he/she shouldn't be guard.
    if(day == mokusoDay && hour >= mokusoTimeStart && hour <= mokusoTimeEnd && candidate.isTakingMokuso) return false;
    // if it's brown exam time and the candidate takes it, he/she shouldn't be guard.
    if(day == brownExamDay && hour >= brownExamTimeStart && hour <= brownExamTimeEnd && candidate.isTakingBrownExam) return false; 
    // if it's physical exam time and the candidate takes it, he/she shouldn't be guard.
    if(day == physicalExamDay && hour >= physicalExamTimeStart && hour <= physicalExamTimeEnd && candidate.isTakingExam) return false; 

    return true;
}


int main (){

    //main parameters
    //start and end of guarding
    int start_day = 1;
    int start_time = 12;
    int end_day = 5;
    int end_time = 14;

    //exception hours
    brownExamDay = 4;
    brownExamTimeStart = 15;
    brownExamTimeEnd = 17;

    mokusoDay = 4;
    mokusoTimeStart = 15;
    mokusoTimeEnd = 17;

    physicalExamDay = 5;
    physicalExamTimeStart = 6;
    physicalExamTimeEnd = 8;

    // no-underage night hours
    nightStartTime = 22;
    nightEndTime = 6;

    ifstream inputParameters;
    inputParameters.open("inputParameters.csv");
    inputParameters.ignore(numeric_limits<streamsize>::max(),'\n');
    string lineParam = "";

    while (getline(inputParameters,lineParam)){

        string tempForNumbers;
        stringstream inputParameters(lineParam);


    }



    unordered_map<int,nameAttr> names;
    ifstream inputFile;
    inputFile.open("inputFile.csv");
    inputFile.ignore(numeric_limits<streamsize>::max(),'\n');
    string line = "";
    int nameId = 0;
    nameAttr inputName1;

    while (getline(inputFile,line)){

        string tempForNonStringElements;
        stringstream inputString (line);
        nameAttr inputName;

        getline(inputString, inputName.name, ',');
        getline(inputString, tempForNonStringElements, ',');
        inputName.rank = atoi(tempForNonStringElements.c_str());

        getline(inputString, tempForNonStringElements, ',');
        inputName.masterOrStudent = tempForNonStringElements[0];

        getline(inputString, tempForNonStringElements, ',');
        istringstream(tempForNonStringElements) >> boolalpha >> inputName.isTakingExam;

        getline(inputString, tempForNonStringElements, ',');
        istringstream(tempForNonStringElements) >> boolalpha >> inputName.isTakingBrownExam;

        getline(inputString, tempForNonStringElements, ',');
        istringstream(tempForNonStringElements) >> boolalpha >> inputName.isUnderage;

        getline(inputString, tempForNonStringElements, ',');
        istringstream(tempForNonStringElements) >> boolalpha >> inputName.isTakingMokuso;

        names[nameId] = inputName;
        nameId++;
        line = "";
    }

    vector<guard> guards;

    for (auto i = names.begin(); i != names.end(); i++){
        if (i->second.masterOrStudent == 't'){
            guard actName;
            actName.nameId = i->first;
            actName.hasBeenGuardCnt = 0;
            actName.hasBeenNightGuardCnt = 0;
            guards.push_back(actName);
        }
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
    shuffle(guards.begin(),guards.end(),e);

    vector<guard_time> pairs;
    bool nightTime = false;

    for (int day = start_day; day <= end_day; day++){
        int first_hour = 0, last_hour = 22;
        if (day == start_day) first_hour = start_time;
        if (day == end_day) last_hour = end_time;
        for (int hour = first_hour; hour <= last_hour; hour = hour + 2){
            if (hour >= 22 || hour <= 4) nightTime = true;
            else nightTime = false;
            guard_time actPair;
            actPair.day = day;
            actPair.hour_time = hour;
            int minGuardTimeCnt = guards[0].hasBeenGuardCnt;
            int minNightGuardTimeCnt = guards[0].hasBeenNightGuardCnt;
            int minNameID = guards[0].nameId;
            int minIndex = 0;
            // name1
            for (auto i = guards.begin(); i != guards.end(); i++){
                if ((i->hasBeenGuardCnt < minGuardTimeCnt) && isAllowed(i->nameId,names,day,hour,nightTime)){
                    minGuardTimeCnt = i->hasBeenGuardCnt;
                    minNameID = i->nameId;
                    minIndex = distance(guards.begin(),i);
                }
                if (nightTime && i->hasBeenGuardCnt == minGuardTimeCnt && i->hasBeenNightGuardCnt < minNightGuardTimeCnt && isAllowed(i->nameId,names,day,hour,nightTime)){
                    minNightGuardTimeCnt = i->hasBeenNightGuardCnt;
                    minNameID = i->nameId;
                    minIndex = distance(guards.begin(),i);
                }
            }
            actPair.name1 = names[minNameID].name;
            guards[minIndex].hasBeenGuardCnt++;
            if (nightTime) guards[minIndex].hasBeenNightGuardCnt++;
            // name2
            minGuardTimeCnt = guards[0].hasBeenGuardCnt;
            minNightGuardTimeCnt = guards[0].hasBeenNightGuardCnt;
            minNameID = guards[0].nameId;
            minIndex = 0;
            for (auto i = guards.begin(); i != guards.end(); i++){
                if ((i->hasBeenGuardCnt < minGuardTimeCnt) && isAllowed(i->nameId,names,day,hour,nightTime)){
                    minGuardTimeCnt = i->hasBeenGuardCnt;
                    minNameID = i->nameId;
                    minIndex = distance(guards.begin(),i);
                }
                if (nightTime && i->hasBeenGuardCnt == minGuardTimeCnt && i->hasBeenNightGuardCnt < minNightGuardTimeCnt && isAllowed(i->nameId,names,day,hour,nightTime)){
                    minNightGuardTimeCnt = i->hasBeenNightGuardCnt;
                    minNameID = i->nameId;
                    minIndex = distance(guards.begin(),i);
                }
            }
            actPair.name2 = names[minNameID].name;
            guards[minIndex].hasBeenGuardCnt++;
            if (nightTime) guards[minIndex].hasBeenNightGuardCnt++;

            pairs.push_back(actPair);          
        }
    }    

    ofstream outputFile;
    outputFile.open("outputFile.csv");
    outputFile  << "Nap,Óra,Név1,Név2\n";
    for (auto i : pairs){
        outputFile  << i.day << "," << i.hour_time << "," << i.name1 << "," << i.name2 << "\n";
    }


    outputFile.close();
 
    return 0;
};
