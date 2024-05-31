#include "tour_generator.h"
#include "geotools.h"
using  namespace std;
TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router){
    geodata_ = &geodb;
    m_route=&router;
}

TourGenerator::~TourGenerator(){

}

std::vector<TourCommand>TourGenerator:: generate_tour(const Stops& stops) const{
    vector<TourCommand> returnVec;
    string point,nextP;
    string talking,nextT;
    vector<GeoPoint> routeTogo;
    GeoPoint currSTP,nextSTP;
    for(int i=0;i<stops.size()-1;++i){
        stops.get_poi_data(i,point, talking);
        stops.get_poi_data(i+1, nextP, nextT);
        TourCommand cmd;
        cmd.init_commentary(point,talking); 

        returnVec.push_back(cmd);
        if(i!=stops.size()-1){
            if(!geodata_->get_poi_location(point,currSTP)){

            }
            if(!geodata_->get_poi_location(nextP,nextSTP)){
                // No such POI in  the database
            }
            routeTogo = m_route->route(currSTP,nextSTP);
            if(routeTogo.empty()) {
                // No Way to go
            }else{
                for(int j=0;j<routeTogo.size()-2;++j){
                    double lineAngle = angle_of_line(routeTogo[j],routeTogo[j+1]);
                    double turnAngle = angle_of_turn(routeTogo[j],routeTogo[j+1],routeTogo[j+2]);
                    string dir,turn;
                    if(lineAngle>=0&&lineAngle<22.5){
                        dir = "east";
                    }else if(lineAngle>=22.5&&lineAngle<67.5){
                        dir = "northeast";
                    }else if(lineAngle>=67.5&&lineAngle<112.5){
                        dir = "north";
                    }else if(lineAngle>=112.5&&lineAngle<157.5){
                        dir = "northwest";
                    }else if(lineAngle>=157.5&&lineAngle<202.5){
                        dir = "west";
                    }else if(lineAngle>=202.5&&lineAngle<247.5){
                        dir = "southwest";
                    }else if(lineAngle>=247.5&&lineAngle<292.5){
                        dir = "south";
                    }else if(lineAngle>=292.5&&lineAngle<337.5){
                        dir = "southeast";
                    }else if(lineAngle>=337.5){
                        dir = "east";
                    }

                    /*
                    if(turnAngle>=0&&turnAngle<22.5){
                        turn = "east";
                    }else if(turnAngle>=22.5&&turnAngle<67.5){
                        turn = "northeast";
                    }else if(turnAngle>=67.5&&turnAngle<112.5){
                        turn = "north";
                    }else if(turnAngle>=112.5&&turnAngle<157.5){
                        turn = "northwest";
                    }else if(turnAngle>=157.5&&turnAngle<202.5){
                        turn = "west";
                    }else if(turnAngle>=202.5&&turnAngle<247.5){
                        turn = "southwest";
                    }else if(turnAngle>=247.5&&turnAngle<292.5){
                        turn = "south";
                    }else if(turnAngle>=292.5&&turnAngle<337.5){
                        turn = "southeast";
                    }else if(turnAngle>=337.5){
                        turn = "east";
                    }
                    */

                    if(turnAngle>=1 && turnAngle < 180){
                        turn = "left";
                    }else if(turnAngle >= 180 && turnAngle <= 359){
                        turn = "right";
                    }
                        //Proceed!
                    TourCommand pcmd;
                    pcmd.init_proceed(dir,geodata_->get_street_name(routeTogo[j],routeTogo[j+1]),
                    distance_earth_miles(routeTogo[j],routeTogo[j+1]),routeTogo[j],routeTogo[j+1]);
                    returnVec.push_back(pcmd);
                    // turn if needed!
                    if(geodata_->get_street_name(routeTogo[j],routeTogo[j+1])!=geodata_->get_street_name(routeTogo[j+1],routeTogo[j+2])){
                        if(turnAngle!=0){
                            TourCommand  tcmd;
                            tcmd.init_turn(turn,geodata_->get_street_name(routeTogo[j+1],routeTogo[j+2]));
                            returnVec.push_back(tcmd);
                        }
                    }
                    
                }
                TourCommand plcmd;
                double lineAngle = angle_of_line(routeTogo[routeTogo.size()-2],routeTogo[routeTogo.size()-1]);
                    string dir;
                    if(lineAngle>=0&&lineAngle<22.5){
                        dir = "east";
                    }else if(lineAngle>=22.5&&lineAngle<67.5){
                        dir = "northeast";
                    }else if(lineAngle>=67.5&&lineAngle<112.5){
                        dir = "north";
                    }else if(lineAngle>=112.5&&lineAngle<157.5){
                        dir = "northwest";
                    }else if(lineAngle>=157.5&&lineAngle<202.5){
                        dir = "west";
                    }else if(lineAngle>=202.5&&lineAngle<247.5){
                        dir = "southwest";
                    }else if(lineAngle>=247.5&&lineAngle<292.5){
                        dir = "south";
                    }else if(lineAngle>=292.5&&lineAngle<337.5){
                        dir = "southeast";
                    }else if(lineAngle>=337.5){
                        dir = "east";
                    }
                    plcmd.init_proceed(dir,geodata_->get_street_name(routeTogo[routeTogo.size()-2],routeTogo[routeTogo.size()-1]),
                    distance_earth_miles(routeTogo[routeTogo.size()-2],routeTogo[routeTogo.size()-1]),routeTogo[routeTogo.size()-2],routeTogo[routeTogo.size()-1]);
                    returnVec.push_back(plcmd);
                //second to last Proceed

                //do nothing for last point
            }
        }
    }
    // last commentary, and do nothing after
    stops.get_poi_data(stops.size()-1,point, talking);
        TourCommand cmd;
        cmd.init_commentary(point,talking);
        returnVec.push_back(cmd);

    
    return returnVec;
}