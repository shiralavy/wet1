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

    /***********************************************************
    * calc_BF: calculates the balance factor - the difference in height between
    * the left and right son-trees of a node in the AVL tree containing the teams
    * @param team the current node
    ***********************************************************/
    int calc_BF(shared_ptr<Team> team);

    /***********************************************************
    * calc_height: calculates the heignt of a node in the AVL tree containing the teams
    ***********************************************************/
    int calc_height(shared_ptr<Team> team);

    /***********************************************************
    * find_team: finds a specific team in the AVL tree containing the teams
    * @param nodeTeam a node in the tree
    * @param teamID the team id number 
    ***********************************************************/
    shared_ptr<Team> find_team(shared_ptr<Team> nodeTeam, int teamID);

    /***********************************************************
    * insert_team: inserts a new team to the AVL tree containing the teams
    * @param nodeTeam a node in the tree
    * @param teamID the new team id number 
    * @param teamID the new team points value
    ***********************************************************/
	  shared_ptr<Team> insert_team(shared_ptr<Team> nodeTeam, int teamID, int points);

    /***********************************************************
    * max: calculates the max between two integers
    * @param a first int
    * @param b second int 
    ***********************************************************/
    static int max(int a, int b);


};

#endif //TEAMS_H_
