#include <room_navigation/room_navigation.h>

RoomNavigation::RoomNavigation():desired_direction_(0), recovery_direction_threshold_(1.0), 
correction_direction_threshold_(0.06), desired_distance_(0), goal_type_(-1)
{
}

RoomNavigation::~RoomNavigation()
{
}

void RoomNavigation::setRecoveryDirectionThreshold(double recovery_direction_threshold)
{
    recovery_direction_threshold_ = recovery_direction_threshold;
}

void RoomNavigation::setCorrectionDirectionThreshold(double correction_direction_threshold)
{
    correction_direction_threshold_ = correction_direction_threshold;
}

void RoomNavigation::setNominalVelocity(double nominal_velocity)
{
    nominal_velocity_ = nominal_velocity;
}

double RoomNavigation::getDesiredDirection()
{
    return desired_direction_; 
}

bool RoomNavigation::determineDirection(double &computed_direction, double curr_direction, double left_ref_direction, 
                                            double left_ref_range, double right_ref_direction, double right_ref_range)
{
       
    if (fabs(desired_direction_ - curr_direction) < recovery_direction_threshold_)
    { 
        // NOTE: both the ranges greater then 0 indicates both are valid ref angles 
        if(left_ref_range > 0  && right_ref_range > 0)
            computed_direction = (left_ref_direction + right_ref_direction)/2.0;
        else if (left_ref_range > 0)
            computed_direction = curr_direction + left_ref_direction;  
        else if (right_ref_range > 0)
            computed_direction = curr_direction + right_ref_direction;
        return true;
    }
    else
        return false;
      // enable recovery behaviour i.e stop the monitors until robot comes back in valid direction range
}

bool RoomNavigation::isCorrectDirection(double left_ref_direction, double left_ref_range, 
                                            double right_ref_direction, double right_ref_range)
{
    if(left_ref_range > 0  && right_ref_range > 0)
    {
        if (fabs(left_ref_direction - right_ref_direction) < correction_direction_threshold_)
            return true;
    }
    return false;
}

bool RoomNavigation::isGoalReached(Gateways detected_gateways, double monitored_distance, double monitored_heading)
{
    if( monitored_distance > (0.8 * desired_distance_) && monitored_distance < (1.2 * desired_distance_))
    {
        if (monitored_heading > (desired_direction_ - 0.4) && monitored_heading < (desired_direction_ + 0.4))
        {
            if (goal_type_ == 0 && (detected_gateways.t_junction.left_turn_range > 0 || detected_gateways.t_junction.right_turn_range > 0))
                return true;
            else if (goal_type_ == 1 && detected_gateways.x_junction.left_turn_range > 0 && detected_gateways.x_junction.right_turn_range > 0)
                return true;

            // TODO: implement right/left door navigation goals
        }
    }  
    return false;
}

double RoomNavigation::computeVelocity(double monitored_distance, double monitored_heading)
{
    double velocity = nominal_velocity_;

    if( monitored_distance > (0.8 * desired_distance_) && monitored_distance < (1.2 * desired_distance_))
    {
        velocity = 0.7 * nominal_velocity_;
    }  

    if (monitored_heading < (desired_direction_ - 0.4) || monitored_heading > (desired_direction_ + 0.4))
    {
        velocity = 0.5 * nominal_velocity_;
    }
    
    return velocity;
}

void RoomNavigation::setGoal(int goal, double direction, double distance)
{
    goal_type_ = goal;
    desired_direction_ = direction;
    desired_distance_ = distance;
}

void RoomNavigation::reset()
{
    desired_direction_ = 0;
    desired_distance_ = 0;
    goal_type_ = -1;
}