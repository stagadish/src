/**!
  * Task.h
  * 
  * /author: Anoop Aroor
  *
  *          Defines tasks as simple target and stores its status and statistics of completion
  */

#ifndef TASK_H
#define TASK_H

#include "definitions.h"
#include "FORRAction.h"
#include "FORRGeometry.h"
#include "Position.h"
#include <vector>
#include <map>
#include <algorithm>

class Task {
  
 public:
  
  Task(int x_in, int y_in)  
    {
      x = x_in;
      y = y_in; 
      status_ = ENROUTE; 
      decision_count = 0;
      decisionSequence = new std::vector<FORRAction>;
      pos_hist = new vector<Position>();
      laser_hist = new vector< vector<CartesianPoint> >();
    }

  int getX() { return x; }
  
  int getY() { return y; }
  
  TASK_STATUS getStatus() { return status_; }
  
  void setStatus(TASK_STATUS stat) { status_ = stat; }

  int getDecisionCount(){return decision_count;} 
 
  int incrementDecisionCount() {decision_count += 1;}

  std::vector<FORRAction> getPreviousDecisions(){
	return *decisionSequence;
  }

  FORRAction saveDecision(FORRAction decision){
	decisionSequence->push_back(decision);
  }

  vector<Position> *getPositionHistory(){return pos_hist;}

  void clearPositionHistory(){pos_hist->clear();}

  void saveSensor(Position currentPosition, vector<CartesianPoint> laserEndpoints){
	if(pos_hist->size() < 1){
		pos_hist->push_back(currentPosition);
		laser_hist->push_back(laserEndpoints);
	}
	else{	
     		Position pos = pos_hist->back();
     		if(!(pos == currentPosition)) {
			pos_hist->push_back(currentPosition);
			laser_hist->push_back(laserEndpoints);
		}
	}
  }

  vector< vector <CartesianPoint> > *getLaserHistory(){return laser_hist;}

 private:
  
  //<! expected task execution time in seconds 
  float time_taken;

  // distance covered by the robot to get to the target
  float distance_travelled; 

  // Sequence of decisions made while pursuing the target
  std::vector<FORRAction> *decisionSequence;

  // Position History : Set of all unique positions the robot has been in , while pursuing the target
  std::vector<Position> *pos_hist; 

  // Cleaned Position History : Set of cleaned positions
  std::vector<Position> *cleaned_pos_hist;

  // Laser scan history 
  vector< vector<CartesianPoint> > *laser_hist; 

  // decision count
  int decision_count;

  // t1, t2, t3 counters
  int tier1_decisions, tier2_decisions, tier3_decisions;

  //<! The point in the map, that the robot needs to go in order to execute this task 
  float x,y;
  
  //<! States the execution status of the task
  TASK_STATUS status_; 
  
};

#endif
