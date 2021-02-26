#ifndef A_STAR_NODE_H
#define A_STAR_NODE_H
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <limits.h>
#include <boost/filesystem.hpp>
#include "rapidjson/document.h"

#include "route_service_msgs/msg/waypoint.hpp"
#include "route_service_msgs/msg/waypoint_array.hpp"
#include "route_service_msgs/srv/route.hpp"

#define PI 3.14159265
#define ALT_DIFF 1




namespace A_star{

class Node{
    public:
        Node();
        Node(const Node &);
        void setNode(std::string,double,double,float);
        void setConnection(Node &);
        void setCost(float);
        void setRoute(std::string);
        void copyNode(const Node &);
        void printout();
        std::string getName();
        double getLatitude();
        double getLongitude();
        float getAltitude();
        std::vector<Node> getConnection();
        std::vector<float> getConnectionCost();
        std::vector<std::string> getConnectionRoute();
    private:
        std::string name;
        double latitude;
        double longitude;
        float altitude;
        std::vector<Node> connect_list;
        std::vector<float> cost_list;
        std::vector<std::string> route_list;
};

struct NodeCosts{
    Node node;
    float F_cost;
    float G_cost;
    float H_cost;
    Node parent;
};


}
#endif
