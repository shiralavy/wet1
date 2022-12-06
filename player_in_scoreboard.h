#ifndef PLAYERS_IN_SCOREBOARD_H_
#define PLAYERS_IN_SCOREBOARD_H_

#include <stdexcept>
//#include <memory>
#include "player.h"
#include "AVLtree.h"


using namespace std;


class player_in_scoreboard {
    public:
    int m_player_id;
    //int m_team_id;
    //int m_games_played;
    //int m_goals;
    //int m_cards;
    //bool m_goalkeeper;
    Node<player>* m_player;
    Node<player_in_scoreboard>* m_prev_player_by_score; //inside linked list sorted by player score
    Node<player_in_scoreboard>* m_next_player_by_score; //inside linked list sorted by player score

   /***********************************************************
        * player_in_scoreboard: constructor for player_in_scoreboard
        *  
        * @param m_player_id the player's unique id         
        * @param m_team_id the player's team
        * @param m_games_played number of games this player has player
        * @param m_goals number of goals this player scored
        * @param m_cards number of cards this played has
        * @param m_goalkeeper boolean value if this player can be a goalkeeper
        * @param m_player a pointer to the player this node represents 
        * @param m_prev_player_by_score a pointer to the previous player in the linked list of players sorted by score
        * @param m_next_player_by_score a pointer to the next player in the linked list of players sorted by score
    ***********************************************************/
    /*
    player_in_scoreboard(int player_id, int team_id, int games_played, int goals, int cards, bool goalkeeper, Node<player>* player = nullptr) : 
    m_player_id(player_id), m_team_id(team_id), m_games_played(games_played), m_goals(goals), m_cards(cards), m_goalkeeper(goalkeeper), 
    m_player(player), m_prev_player_by_score(nullptr), m_next_player_by_score(nullptr)
    {};
    */
    player_in_scoreboard(int player_id) : m_player_id(player_id), m_player(nullptr), m_prev_player_by_score(nullptr), m_next_player_by_score(nullptr)
    {};
    

    /***********************************************************
    * ~player_in_scoreboard: destructor for player_in_team
    ***********************************************************/

    ~player_in_scoreboard(){
        m_prev_player_by_score = nullptr;
        m_next_player_by_score = nullptr;
        m_player = nullptr;
      //  delete m_player;
      //  delete m_prev_player_by_score;
      //  delete m_next_player_by_score;
    }
};

#endif //PLAYERS_IN_SCOREBOARD_H_