#ifndef rt_h
#define rt_h
#include "base_classes.h"
#include "HashMap.h"
#include <map>
#include <vector>
#include <string>
#include <queue>

class Router: public RouterBase
{
public:
 Router(const GeoDatabaseBase& geo_db);
 virtual ~Router();
 virtual std::vector<GeoPoint> route(const GeoPoint& pt1,const GeoPoint& pt2) const;

 private:
struct pairC{
    pairC(){

    }
    pairC(GeoPoint P_, double PD_, double AD_) :P(P_),preCost_(PD_),laterCost_(AD_){
        cost_ = preCost_ + laterCost_;
    }
  GeoPoint P;
  double cost_;
  double preCost_, laterCost_;
  bool operator<(const pairC& other) const {
        return cost_ > other.cost_; // Change to > for a min-heap
    }
};
 const GeoDatabaseBase* data_;


};


#endif