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