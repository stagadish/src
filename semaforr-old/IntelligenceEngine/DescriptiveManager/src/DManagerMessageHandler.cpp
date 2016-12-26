/* DManagerMessageHandler.cpp*
 * Handle messages coming in to the descriptive manager.
 *
 * Copyright 2013 HRTeam.
 * License:
 * Created: Dec 6,2013
 * Original authors: Anoop Aroor <anoop.a.rao@gmail.com>
 */

#include "DManagerMessageHandler.h"
#include "definitions.h"
#include <string>
#include <time.h>
#include <stdio.h>

DManagerMessageHandler::DManagerMessageHandler(CommunicationManager *comm_mgr,DescriptiveManager *desc_manager)
    : MessageHandler(comm_mgr) {
        desc_mgr = desc_manager;
	//std::cout << "Registering callbacks" << std::endl;
	std::cout.flush();
	register_callbacks();
}

void DManagerMessageHandler::register_callbacks() {

  register_callback(CMD_POSE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_pose_handler));
  register_callback(CMD_ASKPOSE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_askpose_handler));
  register_callback(CMD_LAST_ACTION_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_last_action_handler));
  register_callback(CMD_CURRENT_TARGET_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_current_target_handler));
  //register_callback(CMD_ADVISOR_WEIGHT_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_advisor_weight_handler));
  //message handlers dealing with requests for descriptives
  register_callback(CMD_ASK_TARGET_POINT_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_ask_target_point_handler));
  register_callback(CMD_ASK_DISTANCE_TO_TARGET_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_ask_distance_to_target_handler));
  register_callback(CMD_ASK_DISTANCE_FROM_OBSTACLES_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_ask_distance_from_obstacles_handler));
  register_callback(CMD_ASK_LAST_ACTION_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_ask_last_action_handler));
  register_callback(CMD_ASK_ADVISOR_WEIGHT_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_ask_advisor_weight_handler));
  register_callback(CMD_ASK_TEAM_POSE_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_ask_team_pose_handler));
  register_callback(CMD_ASK_ADVISOR_DATA_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_ask_advisor_data_handler));
  register_callback(CMD_ASK_VETO_FORWARD_MOVES_DATA_DESCRIPTIVE, static_cast<MessageCallback>(&DManagerMessageHandler::cmd_veto_forward_moves_advisor_data_handler));
}


void DManagerMessageHandler::cmd_current_target_handler(Message msg) {

time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM: received a current target message which is forwarded by the central server and will store information somewhere" << std::endl;
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  std::vector<std::string> args = msg.get_args();
  
  string type,parameters,value; 
  type = "current_target";
  parameters = robot;
  value = args[0] + " " + args[1];
  //cout << "Details from the message: Type:" << type << "Parameters: " << parameters << "Values: " << value << endl; 
  desc_mgr->update_raw_details(type, parameters, value);
}

void DManagerMessageHandler::cmd_pose_handler(Message msg) {

  time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM: received a pose message which is forwarded by the central server and will store information somewhere" << std::endl;
  string type,parameters,value; 
  std::vector<std::string> args = msg.get_args();
  type = "position";
  parameters = args[0];
  value = args[1];
  for(int i = 2; i < args.size() ; i++)
  {
   value = value + " " + args[i];
  }
  //cout << "Details from the message: Type: " << type << " Parameters: " << parameters << " Values: " << value << endl; 
  desc_mgr->update_raw_details(type, parameters, value);
}
/*
void DManagerMessageHandler::cmd_advisor_weight_handler(Message msg) {

  time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  std::cout << "DM: received a advisor weight message which is forwarded by the central server and will store information somewhere" << std::endl;
  string type,parameters,value; 
  std::vector<std::string> args = msg.get_args();
  type = "weights";
  parameters = args[0];
  value = args[1];
  for(int i = 2; i < args.size() ; i++)
  {
   value = value + " " + args[i];
  }
  cout << "Details from the message: Type: " << type << " Parameters: " << parameters << " Values: " << value << endl; 
  desc_mgr->update_raw_details(type, parameters, value);
}
*/

void DManagerMessageHandler::cmd_askpose_handler(Message msg) {

  time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM: received a ASKPOSE message which is handled but ignored" << std::endl;
}


void DManagerMessageHandler::cmd_last_action_handler(Message msg) {

time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM: received a last action message which is forwarded by the central server and will store information somewhere" << std::endl;
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  std::vector<std::string> args = msg.get_args();
  
  string type,parameters,value; 
  type = "last_action";
  parameters = robot;
  value = args[0] + " " + args[1];
  //cout << "Details from the message: Type:" << type << "Parameters: " << parameters << "Values: " << value << endl; 
  desc_mgr->update_raw_details(type, parameters, value);
}


void DManagerMessageHandler::cmd_ask_team_pose_handler(Message msg){
  time_t rawtime;
  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "team_pose";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for team pose descriptive" << std::endl;
  
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  
  //cout << "Details from the message: Type : " << type << " Robot id: "<< robot << endl;
    
  string descriptive = desc_mgr->get_descriptive(type,robot);
  
  //cout << "After computing descriptive: " << descriptive<< endl;
  send_message(std::string("SINGLE ")+robot+" TEAM_POSE_DESCRIPTIVE "+ descriptive);
}



void DManagerMessageHandler::cmd_ask_target_point_handler(Message msg) {

time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "target_point";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for target point descriptive" << std::endl;
  
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  
  //cout << "Details from the message: Type : " << type << " Robot id: "<< robot << endl;
    
  string descriptive = desc_mgr->get_descriptive(type,robot);
  
  //cout << "After computing descriptive: " << descriptive<< endl;
  if(descriptive == "10000.000000 10000.000000")
  {
    //cout << "no more targets, sending empty message" << endl;
      send_message(std::string("SINGLE ")+robot+" TARGET_POINT_DESCRIPTIVE empty");
  }
  else
  {
      send_message(std::string("SINGLE ")+robot+" TARGET_POINT_DESCRIPTIVE "+ descriptive);
  }
}

void DManagerMessageHandler::cmd_ask_distance_to_target_handler(Message msg) {

time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "distance_from_target";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for distance from target descriptive" << std::endl;
  
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  
  //cout << "Details from the message: Type : " << type << " Robot id: "<< robot << endl;
    
  string descriptive = desc_mgr->get_descriptive(type,robot);
  //cout << "After computing descriptive: " << descriptive<< endl;

  send_message(std::string("SINGLE ")+robot+" DISTANCE_TO_TARGET_DESCRIPTIVE "+descriptive);
}

void DManagerMessageHandler::cmd_ask_distance_from_obstacles_handler(Message msg) {

time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "distance_from_obstacles";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for distance from obstacles descriptive" << std::endl;
  
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  
  //cout << "Details from the message: Type : " << type << " Robot id: "<< robot << endl;
    
  string descriptive = desc_mgr->get_descriptive(type,robot);
  //cout << "After computing descriptive: " << descriptive << endl;

  send_message(std::string("SINGLE ")+robot+" DISTANCE_FROM_OBSTACLES_DESCRIPTIVE "+ descriptive);
}

void DManagerMessageHandler::cmd_ask_last_action_handler(Message msg) {

  time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "last_action";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for last action descriptive" << std::endl;
  
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  
  //cout << "Details from the message: Type : " << type << " Robot id: "<< robot << endl;
    
  string descriptive = desc_mgr->get_descriptive(type,robot);
  //cout << "After computing descriptive: " << descriptive<< endl;

  send_message(std::string("SINGLE ")+robot+" LAST_ACTION_DESCRIPTIVE "+ descriptive);
}

void DManagerMessageHandler::cmd_ask_advisor_weight_handler(Message msg){
  time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "advisor_weight";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for advisor weight descriptive" << std::endl;
  
  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);
  
  //cout << "Details from the message: Type : " << type << " Robot id: "<< robot << endl;
  
  string descriptive = desc_mgr->get_descriptive(type,robot);
  //cout << "After computing descriptive: " << descriptive<< endl;

  send_message(std::string("SINGLE ")+robot+" ADVISOR_WEIGHT_DESCRIPTIVE "+ descriptive);
}

void DManagerMessageHandler::cmd_ask_advisor_data_handler(Message msg){
  time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "advisor_data";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for advisor data descriptive" << std::endl;

  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);

  static int count = 0;
  // Guess have to loop through the vector till all of them are sent
  // maybe create static variable that will count how many times this function
  // have been called
  if(count == desc_mgr->get_number_of_advisors()){
    send_message(std::string("SINGLE ") + robot + " ADVISOR_DATA_DESCRIPTIVE " + " ");
    count = 0;
  }
  else
    send_message(std::string("SINGLE ") + robot + " ADVISOR_DATA_DESCRIPTIVE " + desc_mgr->get_n_description(count++));
}

void DManagerMessageHandler::cmd_veto_forward_moves_advisor_data_handler(Message msg){

 time_t rawtime;

  time (&rawtime);
  printf ("%s", ctime (&rawtime));
  string type = "veto_forward_moves_advisor_data";
  fprintf(stderr, "Got a '%s'\n", msg.get_full_message().c_str());
  //std::cout << "DM : received a request for veto forward moves data descriptive" << std::endl;

  int robot_id = (int)msg.get_sender_id();
  std::string robot = std::to_string(robot_id);

  //cout << "Details from the message: Type : " << type << " Robot id: "<< robot << endl;
    
  string descriptive = desc_mgr->get_descriptive(type,robot);
  //cout << "After computing descriptive: " << descriptive << endl;

  send_message(std::string("SINGLE ") + robot + " VETO_FORWARD_MOVES_DATA_DESCRIPTIVE " + descriptive);
}