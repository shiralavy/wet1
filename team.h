#ifndef TEAM_H_
#define TEAM_H_

#include <stdexcept>
#include <iostream>
#include "player.h"
#include "AVLtree.h"


class Team {
    //private:
public:
    int m_team_id;
    int m_points;
    int m_num_players;
    int m_num_goalkeepers;
    int m_winning_num;
    int m_games_played_by_team;
    //bool m_ready_to_play; //boolean value if this team is ready to play (has 11 players and one goalkeeper)
    AVLtree<player_in_team>* m_tree_players_in_team_by_score;
    AVLtree<player_in_team>* m_tree_players_in_team_by_id;

    /***********************************************************
         * Team: constructor for Team
         * 
         * @param m_team_id the team's team
         * @param m_points number of points this team has
         * @param m_num_players 
         * @param>m_num_goalkeepers
         * @param m_best_player
    ***********************************************************/

    Team(int team_id, int points, int num_players = 0, int num_goalkeepers = 0, int games_played_by_team = 0) : m_team_id(team_id), m_points(points), m_num_players(num_players),m_num_goalkeepers(num_goalkeepers), m_winning_num(points), m_games_played_by_team(games_played_by_team){
        m_tree_players_in_team_by_score = new AVLtree<player_in_team>();
        m_tree_players_in_team_by_id = new AVLtree<player_in_team>();

    };

    /***********************************************************
    * ~Team: destructor for Team
    ***********************************************************/

    ~Team()
    {
        //  std::cout << "Dtor for Team called" << std::endl;

        if (m_tree_players_in_team_by_score) {
            delete m_tree_players_in_team_by_score;
        }
        if (m_tree_players_in_team_by_id)
            delete m_tree_players_in_team_by_id;
    };



    bool check_team_ready()
    {
        if (this->m_num_goalkeepers > 0 && this->m_num_players >= 11){
            return true;
        }
        return false;
    }
};

class ready_team {

public:
    int m_team_id;
    //int m_points;
    //bool m_ready_to_play; //boolean value if this team is ready to play (has 11 players and one goalkeeper)
    Node<Team>* m_team;

    /***********************************************************
         * ready_team: constructor for ready_team
         * 
         * @param m_team_id the team's team
         * @param m_team the corresponding team
    ***********************************************************/

    ready_team(int team_id, Node<Team>* team = nullptr) : m_team_id(team_id), m_team(team){};

    /***********************************************************
    * ~ready_team: destructor for ready_team
    ***********************************************************/

    ~ready_team(){
        m_team = nullptr;
        //delete m_team;
    };
};

#endif //TEAMS_H_
