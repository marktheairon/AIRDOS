#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <boost/filesystem.hpp>
#include <cmath>
#include <limits.h>


#include "rapidjson/document.h"
#include "routeservice.hpp"
#include "a_star_node.h"
#include "route.h"

#include "rclcpp/rclcpp.hpp"

#include "route_service_msgs/msg/waypoint.hpp"
#include "route_service_msgs/msg/waypoint_array.hpp"
#include "route_service_msgs/srv/route.hpp"


boost::filesystem::path dbfolder="/home/airon/Desktop/RouteDatabase";
std::string dbfolder_string="/home/airon/Desktop/RouteDatabase/";

double deg2rad(double deg) {
return (deg * PI / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
return (rad * 180 / PI );
}

double dist_btw_lla_points(double lat_origin,double lon_origin, double lat_far,double lon_far){
    float R=6371*pow(10,3);
    double delta_phi=deg2rad((lat_far-lat_origin));
    double delta_lambda=deg2rad((lon_far-lon_origin));

    double a=sin(delta_phi/2)*sin(delta_phi/2)+
             cos(deg2rad(lat_origin))*cos(deg2rad(lat_far))*
             sin(delta_lambda/2)*sin(delta_lambda/2);

    double c=2*atan2(sqrt(a),sqrt(1-a));
    double d=R*c;

    return d;
}

std::vector<std::string> tokenize_getline(const std::string& data, const char delimiter = '/') {
	std::vector<std::string> result;
	std::string token;
	std::stringstream ss(data);

	while (getline(ss, token, delimiter)) {
		result.push_back(token);
	}
	return result;
}

std::vector<std::string> tokenize_operator(const std::string& data) {
        std::vector<std::string> result;
        std::string token;
        std::stringstream ss(data);

        
        while (ss >> token) {
            result.push_back(token);
        }
        
        return result;
}

std::vector<A_star::Node> parse_node_file(std::string nodefile,std::vector<A_star::Node> &nodelist){
        
        

        std::ifstream in(dbfolder_string+nodefile);
        //std::ifstream in(nodefile);

        char buf[100];

        if (!in.is_open()) {
        std::cout << "파일을 찾을 수 없습니다! check file name" << std::endl;

        }

        std::cout.precision(10);
        int nodenum=0;
        
        while (in) {
        std::string nodename;
        double lat,lon;
        float alt;
        int name;


        in.getline(buf, 100);
        
        if(in.eof()==true){break;}                                             //detect eof and break out, prevents tokenizer seg_fault

        std::vector<std::string> result=tokenize_operator(buf);
        
        if(result[0]=="#"){std::cout<<"pass index row"<<std::endl; continue;}  //erase out first index row  # nodename    lat     lon     alt
        
        for (int i = 0; i < result.size(); i++)
        {                                 
            switch (i)
            {
            case 0:
            nodename =static_cast<std::string>(result[i]);                    //something wrong, set some string variable makes memory error correct
            //std::cout<<nodename<<std::endl;
            break;
            case 1:
            lat=stod(static_cast<std::string>(result[i]));
            //std::cout<<lat<<std::endl; 
            break;

            case 2:
            lon=stod(static_cast<std::string>(result[i]));
            //std::cout<<lon<<std::endl; 
            break;

            case 3:
            alt=stof(static_cast<std::string>(result[i]));
            //std::cout<<alt<<std::endl; 
            break;
            
            default:
            std::cout<<"unexpected coulumn input"<<std::endl;
            break;
            }
        }

        A_star::Node tmp;
        tmp.setNode(nodename,lat,lon,alt);
        nodelist.push_back(tmp);
        //nodelist[nodenum].printout();
        // std::cout<<nodelist[nodenum].getName()<<std::endl;
        nodenum++;
        }

        return nodelist;
}//parse_node_file end

void parse_connection_file(std::vector<A_star::Node> &nodelist)                                         //send nodelist reference
{
  namespace fs=boost::filesystem;                                                               //using boost filesystem
  
  std::vector<fs::path> files;                                                                  //creating path object
   
  //fs::path folder="/home/airon/Desktop/RouteDatabase";                                                           //find current path
 
  fs::path p=dbfolder_string+"connection";                                             //designate connection folder to current path
  //fs::path p="/home/airon/pathDB/connection";    
  fs::directory_iterator it{p};                                                                 //boost create folder iterater object
  
  // connection file example(A-1.txt) 
  
  // #	Connected to	cost	route
  //	Base		0.422	base-a1`	
  //  	A-4		    0.168	a1-a4

  while (it != fs::directory_iterator{})                                                        //while iteration end
  {
      if(it->status().type()==fs::regular_file){                                                //if find regular file, add to files vector
          files.push_back(it->path());
      }
      it++;                                                                                     //to next iteration
  }

  for(auto & cur:files){
      std::cout<<cur.generic_string()<<std::endl;
  }



  for (auto & node :nodelist)                                                                   //for each node in nodelist
  {
    
    for(auto& file: files)                                                                      //for each file in connection folder
    {
     
      if(file.stem()==node.getName())                                                           //if filename matches to nodename
      {     

            std::ifstream cnectfile(file.generic_string());                                     //ifstream, open exact file
            std::string tmp_connected,tmp_cost,tmp_route;                                                 //create temporary variable
            char buf[100];
            std::string bufconvert;
            
            if (!cnectfile.is_open())                                                           //  if cannot open file
            {
                std::cout << "파일을 찾을 수 없습니다! check file name" << std::endl;
            }

            while(cnectfile)                                                                    //while file is open , until eof()
            {
                cnectfile.getline(buf,100);                                                     //write one line to buffer
                bufconvert=static_cast<std::string>(buf);                                       //change char -> string for later use
               
                if(bufconvert.length()==0){break;}                                              //detects blank line, break
                
                if(cnectfile.eof()==true){break;}                                               //detects eof(), break

                std::vector<std::string> result=tokenize_operator(buf);                         //tokenize line

                if(result[0]=="#"){std::cout<<"pass index row"<<std::endl; continue;}           //pass first index line

                for (int i = 0; i < result.size(); i++)                                         // for each element in one line
                {                                 
                    switch (i)
                    {
                    case 0:                                                                     //first element, node name
                        tmp_connected =static_cast<std::string>(result[i]);                   
                        //std::cout<<"parsing elements "<< connected<<"  ";
                        break;
                    case 1:                                                                     //second element, cost
                        tmp_cost=static_cast<std::string>(result[i]);
                        //std::cout<<stof(tmp_cost)<<"  "; 
                        break;

                    case 2:                                                                     //third element, route
                        tmp_route =static_cast<std::string>(result[i]);
                        //std::cout<<route<<std::endl; 
                        break;

                    default:                                                                    //exception, wrong element
                        std::cout<<"unexpected coulumn input"<<std::endl;
                        break;
                    }
                }

                for(auto & current:nodelist)                                                    //searching for all node in nodelist
                {
                    if(tmp_connected==current.getName())                                        //if nodename matches with tmp_connected
                    {   
                        node.setConnection(current);                                            //insert current node to connection_list of mother node 
                        node.setCost(stof(tmp_cost));                                           //insert cost
                        node.setRoute(tmp_route);                                               //insert route
                    }
                }

            }
            //############ DEBUGCODE ###########################
            /*
            std::cout<<"connection nodes: ";
            for(auto & i :connected_nodes){
                    std::cout<<i<<"  ";
                }
            std::cout<<std::endl;
            
            std::cout<<"costs:  ";
            for(auto & i :costs){
                std::cout<<i<<"  ";   
            }
            std::cout<<std::endl;
            
            std::cout<<"routes:  ";
            for(auto & i :routes){
                std::cout<<i<<"  ";   
            }
            std::cout<<std::endl;
            */
            
            //node.setConnection(connected_nodes,costs,routes);
        }

      }
        //node.printout();
    
    }
}                                                                                            

void parse_route_file(std::vector<A_star::Node> &nodelist,std::vector<Route> &routelist)                                        
{
  namespace fs=boost::filesystem;                                                             
  std::vector<fs::path> folders;
  
                                                              
                                                        
  fs::path p=dbfolder.generic_string()+"/route";      
                                        
  fs::directory_iterator it{p};                                                                 

  while (it != fs::directory_iterator{})                                        //collect route directory path                    
  {
      if(it->status().type()==fs::directory_file){                              //if find directory, push back to folder list                               
          folders.push_back(it->path());          
      }
      it++;                                                                                    
  }

  for(int i=0; i<folders.size();i++)
  {                                           //iterate each directory
        std::vector<fs::path> files;
        
        A_star::Node start_tmp, end_tmp;
        //files.push_back(tokenize_getline(folders[i].generic_string()).back());
        fs::directory_iterator iter{folders[i].generic_string()};
        //std::cout<<folders[i].generic_string()<<std::endl;
    
        for(auto & node:nodelist)
        {
            if(node.getName()==folders[i].stem().generic_string())
            {
                start_tmp.copyNode(node);                                           //search nodelist for start node
            }
        }

        
        while (iter != fs::directory_iterator{})                                    //iterate in mother node folder                                  
        {
            if(iter->status().type()==fs::regular_file)
            {                                                
                files.push_back(iter->path());                                      //mother node file list
            }
            iter++;                                                                                   
        }

        for (auto& file: files)                                                                
        {   
            for(auto & node :nodelist)                                                                      
            {      
                if(file.stem()==node.getName())                                                          
                {   
                    Route route;
                    end_tmp.copyNode(node);
                    route.setStartEnd(start_tmp,end_tmp);                           //set route start-end
                    
                    std::cout<<"In searching for mother node:  "<<
                    folders[i].stem()<<"Found exact Node   " <<
                    "File: "<<file.stem()<<"  Node: "<<node.getName()<<std::endl;
                    
                    
                    std::ifstream routefile(file.generic_string()); 
                    std::string tmp_connected,tmp_cost,tmp_route;   
                    
                    std::string bufconvert;


                    while(routefile)                                                                   
                    {   
                        using namespace rapidjson;
                        char buf[65536]={};
                        Waypoint wp_tmp;
                        routefile.read(buf,65536);
                        //std::cout<<buf<<std::endl;
                        Document routebuf;
                        routebuf.Parse(buf);
                        //std::cout<<buf<<std::endl;
                        const Value& mission = routebuf["mission"];
                        //if(mission.IsObject()){std::cout<<"end of route file"<<std::endl;}
                    
                        const Value& items = mission["items"];
                        
                        for(int i=0; i<items.Size();i++)
                        {
                            
                            const Value& waypoint=items[i];
                            const Value& param=waypoint["params"];

                            wp_tmp.param1=param[0].GetFloat();
                            wp_tmp.param2=param[1].GetFloat();
                            wp_tmp.param3=param[2].GetFloat();
                            wp_tmp.param4=NAN;
                            wp_tmp.Lat_x=param[4].GetDouble();
                            wp_tmp.Lon_y=param[5].GetDouble();
                            if(*route.checkUproute()==true)
                            {
                                wp_tmp.Alt_z=param[6].GetDouble()+ALT_DIFF;
                            }
                            else
                            {
                                wp_tmp.Alt_z=param[6].GetDouble()-ALT_DIFF;
                            }
                            
                            wp_tmp.autocontinue=true;
                            wp_tmp.is_current=false;
                            wp_tmp.frame=0;
                            wp_tmp.command=16;
                            route.pushbackWP(wp_tmp);
                        
                        }
                        //printf("%d ", itr->GetInt());

                    } 
                    routelist.push_back(route);
                    

                }

            }      

        }

    } 
}                            

void check_open_close(std::vector<A_star::NodeCosts>openlist ,std::vector<A_star::NodeCosts> closedlist){
    std::cout<<"closedlist:"<<std::endl;
    for(auto &current:closedlist)
    {   
        std::cout<< current.node.getName()<<"   ";
    }
    std::cout<<"    "<<std::endl;
    for(auto &current:closedlist)
    {   
        std::cout<< current.parent.getName()<<"   ";
    }
    std::cout<<std::endl<<std::endl;;
    std::cout<<"openlist:"<<std::endl;
    for(auto &current:openlist)
    {   
        std::cout<< current.node.getName()<<"   ";
    }
    std::cout<<std::endl;

}

float cal_heuristic_cost(A_star::Node now, A_star::Node arrival){
    return dist_btw_lla_points(now.getLatitude(),now.getLongitude(),arrival.getLatitude(),arrival.getLongitude())/1000.0;
}

A_star::Node set_from_nodelist(std::vector<A_star::Node> &nodelist, A_star::Node target){
    for(auto &cur:nodelist)
    {
        if(cur.getName()==target.getName()){
            return cur;
        }
    }
}

void closelist_to_openlist(std::vector<A_star::Node> &nodelist, std::vector<A_star::NodeCosts> &openlist,std::vector<A_star::NodeCosts> &closedlist,A_star::Node & arrival){


            std::vector<A_star::Node> connected_list    =closedlist.back().node.getConnection();           //set 3 vector contain current node's connection information
            std::vector<float> connected_cost   =closedlist.back().node.getConnectionCost();
            std::vector<A_star::NodeCosts> tmplist;

            std::cout<<"connected node information about  :"<<closedlist.back().node.getName()<<std::endl;

            for(int i=0;i<connected_list.size();i++)                                            //for each connection in connected_list calculate full cost and designate parent
            {   
                A_star::NodeCosts opentmp;
                opentmp={set_from_nodelist(nodelist,connected_list[i]),                                         //node
                            0,                                                                                  //Full cost, calculate later
                            connected_cost[i]+closedlist.back().G_cost,                                         //G score, path length
                            cal_heuristic_cost(set_from_nodelist(nodelist,connected_list[i]),arrival),          //H score, direct distance
                            set_from_nodelist(nodelist,closedlist.back().node)                                  //parent node
                            };
                opentmp.F_cost=opentmp.G_cost+opentmp.H_cost;

                std::cout<<"node: "<<opentmp.node.getName()<<"  parent: "<<opentmp.parent.getName()<< "  F  G  H  :"<<opentmp.F_cost<<"    "<<opentmp.G_cost<<"    "<<opentmp.H_cost<<std::endl;
                tmplist.push_back(opentmp);                                                    //create tmplist
            }



            for(int i=0;i<tmplist.size();i++)
            {
                if(tmplist[i].node.getName()==closedlist.back().parent.getName())
                {
                    std::cout<<"Node : "<<tmplist[i].node.getName()<<" is parent node, delete"<<std::endl;
                    tmplist.erase(tmplist.begin()+i);    
                }
            }

/*
           if(closedlist.size()!=1)                                                             //pass parent check at initial openlist set
           { 
                for(int i=0;i<tmplist.size();i++)                                                   //parent check for each element in tmplist
                {
                    for(auto &close_cur:closedlist)                                                 
                    {
                        if(tmplist[i].node.getName()==close_cur.node.getName())                             //if closed list name matches to tmplist element name
                        {
                            
                            tmplist.erase(tmplist.begin()+i);                                       //erase that tmplist element, because it's just reverse route
                        }
                        
                    }
                }
           }
*/



            for(auto &tmp_cur:tmplist)                                                                //Check openlist overlap
            {

                if(openlist.size()!=0)
                {
                    for(auto &open_cur:openlist)
                    {
                        if(open_cur.node.getName()==tmp_cur.node.getName() && open_cur.F_cost>tmp_cur.F_cost)
                        {
                            std::cout<<"Recalculating Cost of : "<<open_cur.node.getName()
                                    <<"   "<<open_cur.F_cost<<"  ->  "<<tmp_cur.F_cost <<std::endl;
                            open_cur.F_cost=tmp_cur.F_cost;
                            open_cur.G_cost=tmp_cur.G_cost;
                            open_cur.H_cost=tmp_cur.H_cost;
                            open_cur.parent=tmp_cur.parent;
                            
                        }
                        else if(open_cur.node.getName()==tmp_cur.node.getName() && open_cur.F_cost<tmp_cur.F_cost)
                        {
                            std::cout<<"Recalculated Cost of :"<<open_cur.node.getName()<<"  But couldn't beat former cost"<<std::endl;
                        }
                        else
                        {
                            openlist.push_back(tmp_cur);
                        }
                    
                    }
                
                }   

                else
                {
                    openlist.push_back(tmp_cur);
                }
            }

     
            
        
}

void cal_min_cost_push(std::vector<A_star::NodeCosts>&openlist,std::vector<A_star::NodeCosts> &closedlist){
    
    std::vector<float> costs;
    
    for(auto &open_cur:openlist)
    {   
        costs.push_back(open_cur.F_cost);
    }
    
    float minimum=costs[0];
    int min_index=0;
    A_star::NodeCosts min_node;
    for (int i = 0; i < costs.size(); ++i)
    {
        if(minimum!=std::min(minimum,costs[i]))
        {
            minimum = std::min(minimum, costs[i]);
            min_index=i;
        }
    }

    check_open_close(openlist,closedlist);

    std::cout<<"Minimum : "<<openlist[min_index].node.getName()<<" added to closed list"<<std::endl;

    closedlist.push_back(openlist[min_index]);
    
    openlist.erase(openlist.begin()+min_index);
    
    check_open_close(openlist,closedlist);

}

int cal_cost(std::vector<A_star::Node> &nodelist, std::vector<A_star::NodeCosts> &openlist,std::vector<A_star::NodeCosts> &closedlist,A_star::Node &departure, A_star::Node &arrival){
   A_star::Node none;
    A_star::NodeCosts tmp={departure,0,0,0,none};
    closedlist.push_back(tmp);


    while(1)
    {
        if(closedlist.back().node.getName()==arrival.getName())                                  //while last element of closedlist is no target
        {   
            std::cout<<"reached to destination!!!"<<std::endl;
            return 1;
            break;
        }
        else
        {
            closelist_to_openlist(nodelist,openlist,closedlist,arrival);
        }


        cal_min_cost_push(openlist,closedlist);
        
        
        //check_open_close(openlist,closedlist);
    }
}


int a_star_path(std::vector<A_star::Node> &nodelist,std::vector<A_star::Node> &path, std::string start, std::string target){
    bool flag1,flag2 =false;
    A_star::Node departure;
    A_star::Node arrival;
    std::vector<A_star::NodeCosts> openlist;                                                            //a* start create open and closed list
    std::vector<A_star::NodeCosts> closedlist;
   

    for(auto &node:nodelist){                                                                   //search all node in nodelist
        if(node.getName()==start){                                                              //if find start node in nodelist
            flag1 =true;                                                                        //indicating found start node
            //departure.copyNode(node);                                                         //Deep Copy node
            departure.copyNode(node);
            std::cout<<"found starting node"<<std::endl;
        }
        else if(node.getName()==target){                                                        //if found target node in nodelist
            flag2 =true;                                                                        //indicating found target node
            //arrival.copyNode(node);
            arrival.copyNode(node);                                                                      //Deep Copy
            std::cout<<"found target node"<<std::endl;
        }
    }
    if(!flag1 || !flag2){return -1;}

                                       

    if(cal_cost(nodelist,openlist,closedlist,departure,arrival))                               //iterate A* open-close list calculation
    {
        for(int closed_cur=closedlist.size()-1;closed_cur>=0;closed_cur--)                      //from closedlist back
        {
            path.push_back(closedlist[closed_cur].parent);
            if(closedlist[closed_cur].parent.getName()==departure.getName()) break;
        }
        path.insert(path.begin(),arrival);
        std::reverse(path.begin(),path.end());
    }
    else
    {

    }
    

    /*
    for(auto & cur:path){
       std::cout<< cur.getName()<<"     ";
    }
    std::cout<<std::endl;*/
                                                                


}



Route search_route(std::vector<Route> &routelist,A_star::Node &start,A_star::Node &end){
    std::vector<Route> searchlist1;

        for(auto & cur:routelist){
            if(cur.startnode()->getName()==start.getName()){
                searchlist1.push_back(cur);
            }
        }

    std::vector<Route> searchlist2;
        for(auto & curr:searchlist1){
            if(curr.endnode()->getName()==end.getName()){
                searchlist2.push_back(curr);
                //std::cout<<"search1 node: "<<curr.endnode()->getName()<<"search2 node: "<<end.getName()<<std::endl;
            }
        }

        if(searchlist2.size()!=1){
            std::cout<<"start-end seach ended in multi path"<<std::endl;
        }
        
        return searchlist2[0];

}


route_service_msgs::msg::WaypointArray waypoint_assembly(std::vector<A_star::Node> &resultpath, std::vector<Route> &routelist){
    
    route_service_msgs::msg::WaypointArray route_final;
    
    for(int i=0;i<resultpath.size()-1;i++){
        A_star::Node start_tmp, end_tmp;
        start_tmp=resultpath[i];
        end_tmp=resultpath[i+1];
        
        std::cout<<"searching for start:  "<<start_tmp.getName()<<"  end:  "<<end_tmp.getName()<<std::endl;
        
        std::vector<Route> searchlist1;

        for(auto & cur:routelist){
            if(cur.startnode()->getName()==start_tmp.getName()){
                searchlist1.push_back(cur);
            }
        }

        std::vector<Route> searchlist2;
        for(auto & curr:searchlist1){
            if(curr.endnode()->getName()==end_tmp.getName()){
                searchlist2.push_back(curr);
                //std::cout<<"search1 node: "<<curr.endnode()->getName()<<"search2 node: "<<end.getName()<<std::endl;
            }
        }

        if(searchlist2.size()!=1){
            std::cout<<"start-end seach ended in multi path"<<std::endl;
        }
        else
        {   
            searchlist2[0].transferWP(route_final);
        }
            
    }

    return route_final;

}






RouteService::RouteService(const rclcpp::NodeOptions & node_options)
: Node("route_service_server", node_options)
{
  RCLCPP_INFO(this->get_logger(), "Run route_service_server");

  this->declare_parameter("qos_depth", 10);
  int8_t qos_depth = 0;
  this->get_parameter("qos_depth", qos_depth);

  const auto QOS_RKL10V =
    rclcpp::QoS(rclcpp::KeepLast(qos_depth)).reliable().durability_volatile();

  route_request_subscriber_ = this->create_subscription<RouteRequest>(
    "route_request",
    QOS_RKL10V,
    [this](const RouteRequest::SharedPtr msg) -> void
    {
      start_= msg->start;
      end_= msg->end;

      RCLCPP_INFO(
        this->get_logger(),
        "Subscribed at: sec %ld nanosec %ld",
        msg->stamp.sec,
        msg->stamp.nanosec);

      RCLCPP_INFO(this->get_logger(), "Subscribed start : %s", start_);
      RCLCPP_INFO(this->get_logger(), "Subscribed end : %s", end_);
    }
  );

  auto route_calculation =
    [this](
    const std::shared_ptr<RouteRequest> request,
    std::shared_ptr<RouteResponse> response) -> void
    {
        
        response->route=this->calculate_route(request->start,request->end);
        response->stamp=this->now();
      
    };

  route_response_server_=
    create_service<RouteOperator>("route_operator", route_calculation);

}

RouteService::~RouteService()
{
}



route_service_msgs::msg::WaypointArray RouteService::calculate_route(const std::string &_start, const std::string &_end)
{
    parsedlist_.clear();
    resultpath_.clear();
    routelist_.clear();
    

    parse_node_file("node.txt",parsedlist_);
  
    parse_connection_file(parsedlist_);
    
    parse_route_file(parsedlist_,routelist_);

    a_star_path(parsedlist_,resultpath_,_start,_end);
    
    resultroute_=waypoint_assembly(resultpath_,routelist_);

    

  return resultroute_;
}




