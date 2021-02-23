#include "route.h"
#include "a_star_node.h"


Route::Route(){
    uproute=true;
}


void Route::setStartEnd(A_star::Node & startnode, A_star::Node & endnode){
    start.copyNode(startnode);
    end.copyNode(endnode);

    double delta_lat=end.getLatitude()-start.getLatitude();
    double delta_lon=end.getLongitude()-start.getLongitude();

    if(delta_lat>0){
        uproute=true;
    }
    else if(delta_lat < 0){
        uproute=false;
    }
    else if(delta_lat==0){
        if(delta_lon>=0){
            uproute=true;
        }
        else uproute=false;
    }
    
}

void Route::pushbackWP(Waypoint wp){
    waypoints.push_back(wp);
}

A_star::Node* Route::startnode(){
    return &start;
}

A_star::Node* Route::endnode(){
    return &end;
}

bool* Route::checkUproute(){
    return &uproute;
}

void Route::printOut(){
    std::cout<<std::endl;
    std::cout<<"Start : "<<start.getName()<<" / End: "<<end.getName()<<std::endl;
    std::cout<<"Waypoints"<<std::endl;
    std::cout<<"Lat              Lon                Alt         param 1  2  3  4    Uproute"<<std::endl;
    for(auto & cur:waypoints){
        std::cout<<cur.Lat_x<<"     "<<cur.Lon_y<<"     "<<cur.Alt_z<<"     "
        <<cur.param1<<"  "<<cur.param2<<"  "<<cur.param3<<"  "<<cur.param4 <<"      "<<uproute<<std::endl;
    }
}

void Route::transferWP(route_service::msg::WaypointArray &target){
    route_service::msg::Waypoint wp_tmp;
    for(int i=0; i<waypoints.size();i++){
        wp_tmp.param1=waypoints[i].param1;
        wp_tmp.param2=waypoints[i].param2;
        wp_tmp.param3=waypoints[i].param3;
        wp_tmp.param4=waypoints[i].param4;
        wp_tmp.lattitude=waypoints[i].Lat_x;
        wp_tmp.longitude=waypoints[i].Lon_y;
        wp_tmp.altitude=waypoints[i].Alt_z;
        wp_tmp.command=waypoints[i].command;
        wp_tmp.iscurrent=waypoints[i].is_current;
        wp_tmp.autocontinue=waypoints[i].autocontinue;
        wp_tmp.frame=waypoints[i].frame;
        target.waypoints.push_back(wp_tmp);
    }

    
}