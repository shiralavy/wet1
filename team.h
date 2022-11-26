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
    //bool m_ready_to_play; //boolean value if this team is ready to play (has 11 players and one goalkeeper)
    AVLtree<Node<player_in_team>> m_tree_players_in_team;

    /***********************************************************
         * Team: constructor for Team
         * 
         * @param m_team_id the team's team
         * @param m_points number of points this team has
    ***********************************************************/

    Team(int team_id, int points) : m_team_id(team_id), m_points(points), m_num_players(0) {};

    /***********************************************************
    * ~Team: destructor for Team
    ***********************************************************/

    ~Team() = default;




};

#endif //TEAMS_H_
