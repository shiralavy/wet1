#ifndef PLAYERS_IN_TEAM_H_
#define PLAYERS_IN_TEAM_H_

#include <stdexcept>
#include <memory>
#include "player.h"
#include "AVLtree.h"


using namespace std;

class player_in_team {
    public:
    int m_player_id;
    int m_team_id;
    int m_games_played;
    int m_goals;
    int m_cards;
    bool m_goalkeeper;
    shared_ptr<Node<player>> m_player;

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
    ***********************************************************/

    player_in_team(int player_id, int team_id, int games_played, int goals, int cards, bool goalkeeper, shared_ptr<Node<player>> player) : 
    m_player_id(player_id), m_team_id(team_id), m_games_played(games_played), m_goals(goals), m_cards(cards), m_goalkeeper(goalkeeper), m_player(player)
    {};

    /***********************************************************
    * ~player_in_team: destructor for player_in_team
    ***********************************************************/

    ~player_in_team() = default;

};

#endif //PLAYERS_IN_TEAM_H_
