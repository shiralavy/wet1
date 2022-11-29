#ifndef TEAM_H_
#define TEAM_H_

#include <stdexcept>
#include "player.h"
#include "AVLtree.h"

class Team {
    //private:
    public:
    int m_team_id;
    int m_points;
    int m_num_players;
    int m_num_goalkeeprs;
    //bool m_ready_to_play; //boolean value if this team is ready to play (has 11 players and one goalkeeper)
    AVLtree<player_in_team>* m_tree_players_in_team;

    /***********************************************************
         * Team: constructor for Team
         * 
         * @param m_team_id the team's team
         * @param m_points number of points this team has
         * @param m_num_players 
         * @param m_num_goalkeeprs
    ***********************************************************/

    Team(int team_id, int points = 0, int num_players = 0, int num_goalkeepers = 0) : m_team_id(team_id), m_points(points), m_num_players(num_players), m_num_goalkeeprs(num_goalkeepers){
        m_tree_players_in_team = new AVLtree<player_in_team>();
    }; 

    /***********************************************************
    * ~Team: destructor for Team
    ***********************************************************/

    ~Team(){
        delete m_tree_players_in_team;
    }; 
    bool check_team_ready()
    {
        if(this->m_num_players>=11)
        {
            return true;
        }
        else
        {
            return false;
        }
        if(this->m_num_goalkeeprs>0)
        {
            return true;
        }
        else
        {
            return false;
        }
        

    }


};

#endif //TEAMS_H_
