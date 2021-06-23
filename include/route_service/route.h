#ifndef ROUTE_H
#define ROUTE_H
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

#include "route_service/a_star_node.h"

struct Waypoint{
    float       param1;
    float       param2;
    float       param3;
    float       param4;
    double      Lat_x;
    double      Lon_y;
    double      Alt_z;
    uint16_t    command;
    bool        is_current;
    bool        autocontinue;
    uint8_t     frame;
};


class Route{
    public:
        Route();
        void setStartEnd(A_star::Node & ,A_star::Node & );
        void pushbackWP(Waypoint);
        A_star::Node* startnode();
        A_star::Node* endnode();
        bool* checkUproute();
        void printOut();
        void transferWP(airdos_msgs::msg::WaypointArray &);



    private:
        A_star::Node start;
        A_star::Node end;
        bool uproute;
        std::vector<Waypoint> waypoints;


};

#endif











