#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "geodb.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include "tour_generator.h"

using namespace std;

void print_tour(vector<TourCommand>& tcs)
{
    double total_dist = 0;
    std::string direction;
    double street_distance = 0;

    cout << "Starting tour...\n";

    for (size_t i = 0; i < tcs.size(); ++i)
    {
        if (tcs[i].get_command_type() == TourCommand::commentary)
        {
            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
            cout << tcs[i].get_commentary() << "\n";
        }
        else if (tcs[i].get_command_type() == TourCommand::turn)
        {
            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
        }
        else if (tcs[i].get_command_type() == TourCommand::proceed)
        {
            total_dist += tcs[i].get_distance();
            if (direction.empty())
                direction = tcs[i].get_direction();
            street_distance += tcs[i].get_distance();
            if (i+1 < tcs.size() && tcs[i+1].get_command_type() == TourCommand::proceed 
                && tcs[i+1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
            {
                continue;
            }

            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
            street_distance = 0;
            direction.clear();
        }
    }

    cout << "Your tour has finished!\n";
    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

/*
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "usage: BruinTour mapdata.txt stops.txt\n";
        return 1;
    }

    GeoDatabase geodb;
    if (!geodb.load(argv[1]))
    {
        cout << "Unable to load map data: " << argv[1] << endl;
        return 1;
    }

    Router router(geodb);
    TourGenerator tg(geodb, router);

    Stops stops;
    if (!stops.load(argv[2]))
    {
        cout << "Unable to load tour data: " << argv[2] << endl;
        return 1;
    }

    std::cout << "Routing...\n\n";

    vector<TourCommand> tcs = tg.generate_tour(stops);
    if (tcs.empty())
        cout << "Unable to generate tour!\n";
    else
        print_tour(tcs);
}
*/


int main()
{
    GeoDatabase geodb;
    cout << "GB loaded"<<endl;
    if (!geodb.load("mapdata.txt"))
    {
        cout << "Unable to load map data: " << endl;
        return 1;
    }

    Router router(geodb);
    cout << "Router Loaded!" << endl;
    TourGenerator tg(geodb, router);
    cout  << "Tour Generator Loaded!" << endl;
    Stops stops;
    if (!stops.load("TESTstops.txt"))
    {
        cout << "Unable to load tour data: " << endl;
        return 1;
    }

    std::cout << "Routing...\n\n";

    vector<TourCommand> tcs = tg.generate_tour(stops);
    if (tcs.empty())
        cout << "Unable to generate tour!\n";
    else{
        cout << "GOGOGO" << endl;
        /*
        for(auto &i : tcs){
            cout <<  i.get_command_type()<< " ";
            if(i.get_command_type()==3){

            }else if(i.get_command_type()==1){
                cout  << i.get_direction() << endl; 
            }else if(i.get_command_type()==2){
                GeoPoint stt;
                GeoPoint ed;
                i.get_points(stt,ed);
                cout << i.get_direction()<<"***"<<i.get_street()<<"  "<<stt.to_string()<<","<<ed.to_string()<<endl;
            }
        }
        cout << "NOW is result" << endl;
        */
        print_tour(tcs);
    }
}
