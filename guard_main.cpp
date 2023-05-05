#include <iostream>
#include <vector>
#include <string>

using namespace std;

// exam times
const int brownExamDay = 4;
const int brownExamTimeStart = 15;
const int brownExamTimeEnd = 17;

const int mokusoDay = 4;
const int mokusoTimeStart = 15;
const int mokusoTimeEnd = 17;

const int physicalExamDay = 5;
const int physicalExamTimeStart = 8;
const int physicalExamTimeEnd = 10;

// no-underage night hours
const int nightStartTime = 22;
const int nightEndTime = 6;


struct nameAttr {
    string name;
    int rank;
    char masterOrStudent;
    bool isTakingExam;
    bool isTakingBrownExam;
    bool isUnderage;
    bool isTakingMokuso;
};

struct guard_time {
    int day;
    int hour_time;
    string name1;
    string name2;
};

vector<nameAttr> populateAllowedNames (vector<nameAttr> names, vector<nameAttr> notAllowedNames){
    vector<nameAttr> allowedNames;
    for (auto i : names) allowedNames.push_back(i);
    for (int i = 0; i < notAllowedNames.size(); i++){
        for (int j = 0; j < allowedNames.size(); j++){
            if (allowedNames[j].name == notAllowedNames[i].name){
                allowedNames.erase(allowedNames.begin() + j);
                break;
            }
        }        
    }
    return allowedNames;
}

vector<nameAttr> populateNotAllowedNames (vector<nameAttr> names, int day, int hour){
    vector<nameAttr> notAllowedNames;
    //masters can't be guards
    for (int i = 0; i < names.size(); i++){
        if (names[i].masterOrStudent == 'm') {
            notAllowedNames.push_back(names[i]);
        }
    }            
    //if it's brown belt exam time, remove related students
    if (day == brownExamDay && hour >= brownExamTimeStart && hour <= brownExamTimeEnd){
        for (int i = 0; i < names.size(); i++){
            if (names[i].isTakingBrownExam){
                notAllowedNames.push_back(names[i]);
            }
        }
    }
    //if it's mokuso time, remove related students
        if (day == mokusoDay && hour >= mokusoTimeStart && hour <= mokusoTimeEnd){
        for (int i = 0; i < names.size(); i++){
            if (names[i].isTakingMokuso){
                notAllowedNames.push_back(names[i]);
            }
        }
    }
    //if it's physical exam time, remove related students
        if (day == physicalExamDay && hour >= physicalExamTimeStart && hour <= physicalExamTimeEnd){
        for (int i = 0; i < names.size(); i++){
            if (names[i].isTakingExam){
                notAllowedNames.push_back(names[i]);
            }
        }
    }
    //if it's night time, underages shouldn't be guards
        if (hour >=  nightStartTime || hour <= nightEndTime){
        for (int i = 0; i < names.size(); i++){
            if (names[i].isUnderage){
                notAllowedNames.push_back(names[i]);
            }
        }
    }
    return notAllowedNames;
}




int main (){

    vector<nameAttr> names = {
    {"Ringhofer Zsolt",1,'m',false,false,false,false},
    {"Ringhofer Csaba",1,'m',false,false,false,false},
    {"Berényi Lilla",8,'s',true,false,false,false},
    {"Sirokai Soma",6,'s',true,false,false,false},
    {"Bacskó Zétény",8,'s',true,false,false,false},
    {"Tóth Marcell",8,'s',true,false,false,false},
    {"Nagy Levente",8,'s',true,false,false,false},
    {"Kecskés Bence",9,'s',true,false,false,true},
    {"Leloup Aaron",9,'s',true,false,true,true},
    {"Horváth Ágoston",9,'s',true,false,false,true},
    {"Kováts Károly",1,'s',false,false,false,false},
    {"Tóth Béla",4,'s',true,true,false,false}
    };
    
    vector<guard_time> pairs;
    int start_day = 1;
    int start_time = 12;
    int end_day = 5;
    int end_time = 14;

    vector<nameAttr> actNames = names;
    vector<nameAttr> notAllowedNames;
    vector<nameAttr> allowedNames;

    for (int day = start_day; day <= end_day; day++){
        int first_hour = 0, last_hour = 22;
        if (day == start_day) first_hour = start_time;
        if (day == end_day) last_hour = end_time;
        for (int hour = first_hour; hour <= last_hour; hour = hour + 2){
            notAllowedNames = populateNotAllowedNames(names,day,hour);
            allowedNames = populateAllowedNames(names,notAllowedNames);
            actNames = populateAllowedNames(actNames,notAllowedNames);
            //if the namelist becomes empty, refill it with everyone allowed                       
            if (actNames.size() == 0) actNames = allowedNames;

            guard_time act;
            act.day = day;
            act.hour_time = hour;

            int name1Index = rand() % actNames.size();
            act.name1 = actNames[name1Index].name;
            actNames.erase(actNames.begin() + name1Index);
            if (actNames.size() == 0) actNames = allowedNames;
            if (actNames.size() > 1){
                for (int i = 0; i < actNames.size(); i++){
                    if (actNames[i].name == act.name1){
                        actNames.erase(actNames.begin() + i);
                        break;
                    }
                }
            }

            int name2Index = rand() % actNames.size();
            act.name2 = actNames[name2Index].name;
            actNames.erase(actNames.begin() + name2Index);
            if (actNames.size() == 0) actNames = allowedNames;
            if (actNames.size() > 1){
                for (int i = 0; i < actNames.size(); i++){
                    if (actNames[i].name == act.name2){
                        actNames.erase(actNames.begin() + i);
                        break;
                    }
                }
            }


            pairs.push_back(act);

            //put back temporarily removed names
            actNames.insert(end(actNames),begin(notAllowedNames),end(notAllowedNames));
            notAllowedNames = {};
        }
    }

    for (auto i : pairs){
        cout  << i.day << " " << i.hour_time << " " << i.name1 << " " << i.name2 << endl;
    }
 
    return 0;
};
