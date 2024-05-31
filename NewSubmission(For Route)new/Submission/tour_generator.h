#ifndef tg_h
#define tg_h
#include "base_classes.h"
#include "HashMap.h"
#include <vector>

class TourGenerator: public TourGeneratorBase
{
public:
 TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
 virtual ~TourGenerator();
 virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;

 private:
 const GeoDatabaseBase* geodata_;
 const RouterBase* m_route;
};

#endif