#ifndef TEAMS_H_
#define TEAMS_H_

#include <stdexcept>
#include "players.h"

class Team {
    //private:
    public:
    int m_team_id;
    int m_points;
    bool m_ready_to_play; //boolean value if this team is ready to play (has 11 players and one goalkeeper)
    int m_height_teams;

    shared_ptr<Player> m_root_players_by_id;

    shared_ptr<Team> m_left_team;
    shared_ptr<Team> m_right_team;

    /***********************************************************
         * Team: constructor for Team
         * 
         * @param m_team_id the team's team
         * @param m_points number of points this team has
    ***********************************************************/

    Team(int team_id, int points) : m_team_id(team_id), m_points(points), m_ready_to_play(false), m_height_teams(0) {};

    /***********************************************************
    * ~Team: destructor for Team
    ***********************************************************/

    ~Team() = default;




};

#endif //TEAMS_H_
