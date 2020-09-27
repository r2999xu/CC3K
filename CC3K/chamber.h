#ifndef __CHAMBER_H__
#define __CHAMBER_H__

#include "posn.h"
#include <vector>
#include <memory>

class Chamber {
    public:
    int index;
    std::vector<Posn> boundary;
    std::vector<Posn> inside;
    std::vector<Posn> doors;

    Chamber(int);
    bool contains(Posn); // Check if the point represented by the Posn parameter is within the boundary
};
#endif