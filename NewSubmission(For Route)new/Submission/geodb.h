#ifndef geodb_h
#define geodb_h
#include "base_classes.h"
#include "HashMap.h"
#include <vector>
#include <string>


class GeoDatabase: public GeoDatabaseBase
{
public:
 GeoDatabase();
 virtual ~GeoDatabase();
 virtual bool load(const std::string& map_data_file);
 virtual bool get_poi_location(const std::string& poi,
 GeoPoint& point) const;
 virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
 const;
 virtual std::string get_street_name(const GeoPoint& pt1,
 const GeoPoint& pt2) const;

 private:
 struct segment{
    segment(){
        
    }
    segment(const std::string slati,const std::string slong, const std::string elati, const std::string elong, std::string name):startP(slati,slong),endP(elati,
    elong){
        streetName = name;

    }
 GeoPoint startP;
 GeoPoint endP;
 std::string streetName;
 };

 struct connect{
    connect(){
        std::cerr << startP.latitude<<std::endl;
    }
    connect(double  lat_, double lon_) :startP(std::to_string(lat_),std::to_string(lon_)){

    }

    connect(const std::string  lat_, const std::string lon_) :startP(lat_,lon_){

    }
    
    GeoPoint startP;
    std::vector<GeoPoint> related;
    std::vector<std::string> names;

 };
HashMap<segment> dataBase; //for street Name
HashMap<connect> connectedPoints; // for connected Points
HashMap<GeoPoint> POIM; // for Point of Interest

};


#endif