#include "router.h"
#include "geotools.h"
#include <algorithm>
using namespace std;

Router::Router(const GeoDatabaseBase& geo_db){
    data_ = &geo_db;
}

Router::~Router(){

}

std::vector<GeoPoint> Router::route(const GeoPoint& pt1,const GeoPoint& pt2) const{
    vector<GeoPoint> returnVec;
    HashMap<GeoPoint> locationOfPreviousWayPoint;
    HashMap<bool> visited;
    if(pt1.latitude==pt2.latitude&&pt1.longitude==pt2.longitude){ // At destination
        returnVec.push_back(pt1);
        return returnVec;
    }
    vector<GeoPoint> nextStep;

    priority_queue<pairC> myQ;
    double toGo, gone, cost;
    nextStep=data_->get_connected_points(pt1); //Pt1's next steps
    
    for(int i=0;i<nextStep.size();++i){
        locationOfPreviousWayPoint.insert(nextStep[i].to_string(),pt1);
        gone = distance_earth_km(pt1,nextStep[i]);
        toGo =  distance_earth_km(nextStep[i],pt2);
        cost = gone + toGo;
        myQ.push(pairC(nextStep[i],gone,toGo));
    }
    nextStep.clear();
    visited.insert(pt1.to_string(),true);
    while(!myQ.empty()) {
        GeoPoint currentPt = myQ.top().P;// visit this poit now
        double  totCost = myQ.top().preCost_; //Only need the cost of getting here
        visited.insert(currentPt.to_string(),true);
        if(currentPt.latitude == pt2.latitude && currentPt.longitude == pt2.longitude) {
            //found the destination
            returnVec.push_back(currentPt);
            string key = locationOfPreviousWayPoint.find(currentPt.to_string())->to_string();
            if(1)returnVec.push_back(*locationOfPreviousWayPoint.find(currentPt.to_string()));
            while(key!=pt1.to_string()){
                returnVec.push_back(*locationOfPreviousWayPoint.find(key));
                key =  locationOfPreviousWayPoint.find(key)->to_string();
            }
            //returnVec.push_back(pt1);
            std::reverse(returnVec.begin(), returnVec.end());
            return  returnVec;
            break; 
        }
        myQ.pop();
        nextStep = data_->get_connected_points(currentPt);// Not done, go to next step
        for(int i=0;i<nextStep.size();++i){
            if(visited.find(nextStep[i].to_string())==nullptr){// Not visited these points
            locationOfPreviousWayPoint.insert(nextStep[i].to_string(),currentPt);
            gone = totCost + distance_earth_km(currentPt,nextStep[i]);// How far have I gone 
            toGo =  distance_earth_km(nextStep[i],pt2);
            cost = gone + toGo;
            myQ.push(pairC(nextStep[i],gone,toGo));
            }
        }
        nextStep.clear();
    }
    return returnVec;
}
