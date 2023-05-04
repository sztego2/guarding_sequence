#include <iostream>
#include <vector>
#include <string>

using namespace std;
struct guard_time {
    int day;
    int hour_time;
    string name1;
    string name2;
};

int main (){

    vector<string> names = {"Ringhofer Zsolt","Ringhofer Csaba","Berényi Lilla","Sirokai Soma","Bacskó Zétény"};
    vector<guard_time> pairs;
    int start_day = 1;
    int start_time = 12;
    int end_day = 5;
    int end_time = 14;

    vector<string> actNames = names;
    for (int day = start_day; day <= end_day; day++){
        int first_hour = 0, last_hour = 22;
        if (day == start_day) first_hour = start_time;
        if (day == end_day) last_hour = end_time;
        for (int hour = first_hour; hour <= last_hour; hour = hour + 2){
            guard_time act;
            act.day = day;
            act.hour_time = hour;

            int name1Index = rand() % actNames.size();
            act.name1 = names[name1Index];
            actNames.erase(actNames.begin() + name1Index);
            if (actNames.size() == 0) actNames = names;

            int name2Index = rand() % actNames.size();
            act.name2 = names[name2Index];
            actNames.erase(actNames.begin() + name2Index);
            if (actNames.size() == 0) actNames = names;

            pairs.push_back(act);
        }
    }

    for (auto i : pairs){
        cout  << i.day << " " << i.hour_time << " " << i.name1 << " " << i.name2 << endl;
    }
      
    return 0;
}