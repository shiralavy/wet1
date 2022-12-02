#ifndef PLAYERS_IN_TEAM_H_
#define PLAYERS_IN_TEAM_H_

#include <stdexcept>
//#include <memory>
#include "player.h"
#include "AVLtree.h"


using namespace std;

class player_in_team {
    public:
    int m_player_id;
    int m_team_id;
    //int m_games_played;
    //int m_goals;
    //int m_cards;
    //bool m_goalkeeper;
    Node<player>* m_player;
    Node<Team>* m_my_team;

   /***********************************************************
         * player_in_team: constructor for player_in_team
         * 
         * @param m_player_id the player's unique id
         * @param m_team_id the player's team
         * @param m_games_played number of games this player has played
         * @param m_goals number of goals this player scored
         * @param m_cards number of cards this played has
         * @param m_goalkeeper boolean value if this player can be a goalkeeper
         * @param m_player a pointer to the player this node represents 
         * @param m_my_team a pointer to the team this player belongs to
    ***********************************************************/

    player_in_team(int player_id, int team_id, Node<player>* player = nullptr, Node<Team>* team = nullptr) : 
    m_player_id(player_id), m_team_id(team_id), m_player(player), m_my_team(team)
    {};

    /***********************************************************
    * ~player_in_team: destructor for player_in_team
    ***********************************************************/

    ~player_in_team(){
        m_player = nullptr;
        m_my_team = nullptr;
        delete m_player;
        delete m_my_team;
    }

};

#endif //PLAYERS_IN_TEAM_H_
