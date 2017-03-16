
#include "Tier1Advisor.h"

/*
void Tier1Advisor::advisorNotOpposite(Beliefs *beliefs){
  cout << "COntroller::advisorNotOpposite > Entering function" << endl;
  vector<FORRAction> actions = beliefs->getPreviousDecisions();
  int size = actions.size();
  if(actions.size() < 2){
    cout <<"actions list less than 2. Exiting not opposite" << endl;
    return;
  }
  FORRAction lastAction = actions[size - 1];
  FORRAction lastlastAction = actions[size - 2];
  cout << "Controller::advisorNotOpposite > " << lastAction.type << " " << lastAction.parameter << ", " << lastlastAction.type << " " << lastlastAction.parameter << endl; 
  if(lastlastAction.type == RIGHT_TURN or lastlastAction.type == LEFT_TURN){
    if(lastAction.type == PAUSE){
      cout << "Not opposite active "<< endl;
      if(lastlastAction.type == RIGHT_TURN)    for(int i = 1; i < 6 ; i++)   (beliefs->vetoedActions)->insert(FORRAction(LEFT_TURN, i));
      else                                     for(int i = 1; i < 6 ; i++)   (beliefs->vetoedActions)->insert(FORRAction(RIGHT_TURN, i));
    }
  }
  cout << "leaving notOpposite"<<endl;
  return;
}


// This advisor should ban all forward moves not in the direction of the exit unless all of the exits
// are already blocked by other robots
void Tier1Advisor::advisorCircle(Beliefs *beliefs){

  // Initialize variables
  vector<FORRCircle> circles = (beliefs->abstractMap).getCircles();
  Position curr_pos = beliefs->getCurrentPosition();
  Task *task = beliefs->getCurrentTask();
  CartesianPoint targetPoint (task->getX() , task->getY());
  CartesianPoint currentPosition (curr_pos.getX(), curr_pos.getY());
  bool targetInCircle = false;
  bool currPosInCircleWithExit = false;
  int robotCircle=-1, targetCircle = -1;
  //cout << "Controller::advisorCircle >> Initializing variables " << endl;
  // check the preconditions for activating the advisor
  for(int i = 0; i < circles.size() ; i++){
    // check if the target point is in circle
    if(circles[i].inCircle(targetPoint.get_x(), targetPoint.get_y())){
      targetInCircle = true;
      targetCircle = i;
    }
    // check if the rob_pos is in a circle and the circle has atleast one exit
    if(circles[i].inCircle(curr_pos.getX(), curr_pos.getY()) and ((circles[i]).getExits().size() >= 1)){
      currPosInCircleWithExit = true;
      robotCircle = i;
    }
  }

  // if preconditions are met, veto forward moves in the direction of non exits
  if(targetInCircle == true and currPosInCircleWithExit == true and robotCircle != targetCircle){
    //cout << "Controller::advisorCircle >> Activating tier 1 get out of circle advisor" << endl;
    vector<FORRExit> exits = circles[robotCircle].getExits(); 
    bool facingExit = false;
    double forward_distance = beliefs->wallDistanceVector[0];
    for(int i = 0; i< exits.size(); i++){
      CartesianPoint exitPoint = exits[i].getExitPoint();
      double exitDistance = Utils::get_euclidian_distance(exitPoint.get_x(), exitPoint.get_y(), curr_pos.getX() , curr_pos.getY());
      if(!(beliefs->abstractMap).isExitToLeaf(exits[i]) || exits[i].getExitCircle() == targetCircle){
	if( ( isFacing(curr_pos, exitPoint, circles[robotCircle].getRadius()) and forward_distance - exitDistance > 20 ) or exitDistance < 40 ){
	  //cout << "Controller::advisorCircle >> Robot is facing exit " << exitPoint.get_x() << exitPoint.get_y() << endl;
	  facingExit = true;
	  break;
	}
      }
    }
    // Robot is not facing any of the exits ban all forward moves
    if(facingExit == false){
      //cout << "Controller::advisorCircle >> Vetoing all forward moves" << endl;
      for(int i = 1; i < 6; ++i){
	(beliefs->vetoedActions)->insert(FORRAction(FORWARD, i));
      }
    }
  }
  return;
}

bool Tier1Advisor::isFacing(Position robotPos , CartesianPoint point, double radius){
  bool isFacing = false;
  double robot_point_angle = atan2(point.get_y() - robotPos.getY(), point.get_x() - robotPos.getX());
  double angleDiff = robotPos.getTheta() - robot_point_angle;
  //cout << "In function isFacing " << endl;
  //cout << "Robot angle " << robotPos.getTheta() << endl;
  //cout << "Angle made by the robot and the position with x axis" << robot_point_angle << endl;
  double ratio = Utils::get_euclidian_distance(robotPos.getX(), robotPos.getY() ,point.get_x(), point.get_y())/(2*radius);
  double min_delta = 0.72; //30 degrees on each side
  double max_delta = 3.14/(2); //60 degrees on each side
  double delta = ratio*(min_delta) + (1-ratio)*(max_delta) ;

  if(abs(angleDiff) < delta){
    isFacing = true;
  }
  return isFacing;
}


*/

bool Tier1Advisor::advisorVictory(FORRAction *decision) {
  cout << "Begin victory advisor" << endl;
  // if the robot is oriented towards the goal and the robot actions which are not vetoed allows the robot to reach the goal then take that action.
  bool decisionMade = false;
  
  // Check if the current heading intersects with any of the walls
  int buffer = 30;
  bool targetNotInSight = beliefs->getAgentState()->isTargetInSight();
  
  if(targetNotInSight == true){
    cout << "Target not in sight , skipping victory advisor" << endl;
    
  }
  else{
      cout << "Target in sight , victory advisor active" << endl;
      (*decision) = beliefs->getAgentState()->singleMoveTowardsTarget();
      decisionMade = true;
  }
  return decisionMade;
}


// Slavisa added Jan. 2014
/*
 * This advisor has to do prevent all the actions that will result in robot hitting the wall.
 * Straight forward thing is to check for collsion in the orientation.
 * Slightly more complicated is to take into an account that robot takes up some space and 
 * it can hit the wall although there is no visible collision near (almost parallel to the wall).
 * Have to make it work and yet be simple computationally.
 */
bool Tier1Advisor::advisorAvoidWalls(){
  const int THRESHOLD1 = 30;
  const int THRESHOLD2 = 45;
  //cout << "Avoid Walls called " << endl;
  double wall_distance = beliefs->getAgentState()->getForwardObstacleDistance();
  //cout << "avoid_walls_advisor:: Received distance from wall descriptive " << wall_distance << endl;
  
  set<FORRAction> *vetoedActions = beliefs->getAgentState()->getVetoedActions();
  
  if(wall_distance - 3 < THRESHOLD1){
    vetoedActions->insert(FORRAction(FORWARD, 1));
    //cout << "Vetoed Forward move with intensity 1 :" << endl;
  }
  if(wall_distance - 7 < THRESHOLD1){
    vetoedActions->insert(FORRAction(FORWARD, 2));
    //cout << "Vetoed Forward move with intensity 2"<< endl;
  }
  if(wall_distance - 20 < THRESHOLD1){
    vetoedActions->insert(FORRAction(FORWARD, 3));
    //cout << "Vetoed Forward move with intensity 3"<< endl;
  }
  if(wall_distance - 25 < THRESHOLD2){
    vetoedActions->insert(FORRAction(FORWARD, 4));
    //cout << "Vetoed Forward move with intensity 4"<< endl;
  }
  if(wall_distance - 105 < THRESHOLD2){
    vetoedActions->insert(FORRAction(FORWARD, 5));
    //cout << "Vetoed Forward move with intensity 5"<< endl;
  }
  for(int i = 1; i < 6; ++i)
    vetoedActions->insert(FORRAction(BACKWARD, i));
  // because it is annoying I have to do this manually
  //vetoed_actions.insert(FORRAction(RIGHT_TURN, 5));
  //vetoed_actions.insert(FORRAction(LEFT_TURN, 5));
  //cout << " return from wall advisor" << endl;
  return false; 
}

