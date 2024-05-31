#include "geodb.h"
#include "geotools.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


GeoDatabase::GeoDatabase(){

}

GeoDatabase::~GeoDatabase(){

}

bool GeoDatabase::load(const std::string& map_data_file){
    ifstream inf(map_data_file);
    if(!inf){ // NO File
        return false;
    }
    string line;
    string streetName;
    int lineCount = 0;

        string word;
		string longs;
        string longe;
        string latis;
        string latie;
	while (getline(inf, line)){
        
        istringstream iss(line); 
        int POI = 0;
        if(lineCount%3==0){ // Got a street Name
            streetName = line;
            //cerr << streetName << "   shabi"<< endl;
            lineCount+=1;
        }else if(lineCount%3==1&&iss>>latis>>longs>>latie>>longe){
            //cerr<< latis << " " << longs << " " <<latie  << " " << longe <<endl;
            segment Q(latis,longs,latie,longe,streetName);

            connect SP(latis,longs);
            connect EP(latie,longe);

            SP.names.push_back(streetName);
            EP.names.push_back(streetName);
            SP.related.push_back(EP.startP);// Start Point is related to End Point, and Vice Versa
            EP.related.push_back(SP.startP);
            // a point exist in map, then just add the new related point to the inner vector
            // otherwise create a new one with this Point as GeoPoint and another Point as its connect Point
            if(connectedPoints.find(SP.startP.to_string())!=nullptr){
                connectedPoints[SP.startP.to_string()].related.push_back(EP.startP);
            }else{
                connectedPoints.insert(SP.startP.to_string(),SP);
            }

            if(connectedPoints.find(EP.startP.to_string())!=nullptr){
                connectedPoints[EP.startP.to_string()].related.push_back(SP.startP);
            }else{
                connectedPoints.insert(EP.startP.to_string(),EP);
            }


            //connectedPoints[SP.startP.to_string()] =SP;
            //connectedPoints[EP.startP.to_string()] =EP;


            //connectedPoints[SP.startP.sLatitude+" "+SP.startP.sLongitude] =SP;
            //connectedPoints[EP.startP.sLatitude+" "+EP.startP.sLongitude] =EP;
            /*
            if(dataBase.find(streetName)!=nullptr){
                if(dataBase.find(streetName)->endP.latitude==latis&&dataBase.find(streetName)->endP.longitude == longs){
                    dataBase.find(streetName)->endP.latitude = latie;
                    dataBase.find(streetName)->endP.longitude = longe;
                }else if(dataBase.find(streetName)->startP.latitude==latie&&dataBase.find(streetName)->startP.longitude == longe){
                    dataBase.find(streetName)->startP.latitude = latis;
                    dataBase.find(streetName)->startP.longitude = longs;
                }
            }else{
            dataBase[streetName]=Q;
            }
            */
            // Either direction can find this street Name
            dataBase.insert(Q.startP.to_string()+Q.endP.to_string(), Q);
            dataBase.insert(Q.endP.to_string()+Q.startP.to_string(),Q);
            lineCount +=1;
        }else if(lineCount%3==2&& iss >> POI){

            if(POI == 0){
                lineCount+=1;
                continue;
            
            }else{
                //cerr << POI <<endl;
                GeoPoint midPoint = midpoint(GeoPoint(latis,longs),GeoPoint(latie,longe));
                // StarP push back mid point in related

                connectedPoints[latis+","+longs].related.push_back(midPoint);//EP and SP connect to MidPoint
                connectedPoints[latie+","+longe].related.push_back(midPoint);

                segment Q1(latis,longs,midPoint.sLatitude,midPoint.sLongitude,streetName);
                segment Q2(midPoint.sLatitude,midPoint.sLongitude,latie, longe,streetName);
                //streetName
                dataBase.insert(Q1.startP.to_string()+Q1.endP.to_string(),Q1);
                dataBase.insert(Q1.endP.to_string()+Q1.startP.to_string(),Q1);
                dataBase.insert(Q2.startP.to_string()+Q2.endP.to_string(),Q2);
                dataBase.insert(Q2.endP.to_string()+Q2.startP.to_string(),Q2);

                connect SPMP(midPoint.sLatitude, midPoint.sLongitude);
                SPMP.names.push_back(streetName);
                SPMP.names.push_back("a path");
                // Midpoint itself is connect to SP and EP
                SPMP.related.push_back(connectedPoints[latis+","+longs].startP);

                SPMP.related.push_back(connectedPoints[latie+","+longe].startP);

                for(int i=0;i<POI;i++){ // Get detailed POIs

                    getline(inf, line);

                    std::string POIName;
                    char check = ' ';
                    std::string  POIlati, POIlon;
                    int j = 0;
                    while(check!='|'){ // before | is the POI Name
                        check = line[j];
                        POIName+=check;
                        j++;
                        check = line[j];
                    }
                    //cerr << POIName << endl;
                    j++;
                    while(check!=' '){
                        check  = line[j];
                        POIlati+=check;
                        j++;
                        check = line[j];
                    }
                    j++;
                    //cerr << POIlati << endl;
                    for(;j<line.length();j++) {
                        POIlon+=line[j];
                    }
                    //cerr << POIlon << endl;
                    POIM.insert(POIName,GeoPoint(POIlati,POIlon));

                    connect POIPoint(POIlati,POIlon);
                    POIPoint.names.push_back("a path");// On a path and connect to Mid Point
                    POIPoint.related.push_back(midPoint);
                    //cerr << "150" << endl;
                    segment Q3(midPoint.sLatitude,midPoint.sLongitude,POIPoint.startP.sLatitude,POIPoint.startP.sLongitude,"a path");
                    //Connect Q3 to POI
                    dataBase.insert(Q3.startP.to_string()+Q3.endP.to_string(),Q3);
                    //cerr <<"OFK"<< endl;
                    dataBase.insert(Q3.endP.to_string()+Q3.startP.to_string(),Q3);
                    //cerr << "154" << endl;
                    SPMP.related.push_back(POIPoint.startP);
                    connectedPoints.insert(POIlati+","+POIlon,POIPoint);
                    //cerr << "157" << endl;
                }
                connectedPoints.insert(SPMP.startP.to_string(),SPMP);
                lineCount +=1; // Go to next Round
            }
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi,GeoPoint& point) const{
    if(POIM.find(poi)==nullptr)return false;
    point = *POIM.find(poi);
    return true;
 }

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const{
    string key = pt.to_string();
    vector<GeoPoint> returnVec;
    if(connectedPoints.find(key)!=nullptr){
        return connectedPoints.find(key)->related;
    }else{
        return returnVec;
    }
 }

std::string GeoDatabase::get_street_name(const GeoPoint& pt1,const GeoPoint& pt2) const{
    std::string returnSTR ="";
    if(dataBase.find(pt1.to_string()+pt2.to_string())==nullptr){
        return returnSTR;
    }else{
        return dataBase.find(pt1.to_string()+pt2.to_string())->streetName;
    }
}
