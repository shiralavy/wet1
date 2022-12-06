#ifndef PLAYERS_H_
#define PLAYERS_H_

//#include "player_in_team.h"
//#include "player_in_scoreboard.h"
#include "AVLtree.h"

using namespace std;

class player_in_scoreboard;
class player_in_team;
class Team;

// WE NEED TO REMEMBER TO CREATE ASSIGNMENT OPERATOR FOR THIS AND ALL OF THE OTHER CLASSES
class player {
    //private:
    public:
    int m_player_id;
    int m_team_id;
    int m_games_played;
    int m_goals;
    int m_cards;
    //int m_closest_player;
    bool m_goalkeeper;
    //shared_ptr<Node<player_in_team>> player_in_team;
    //shared_ptr<Node<player_in_scoreboard>> player_in_scoreboard;
    Node<player_in_team>* m_player_in_team_by_score;
    Node<player_in_team>* m_player_in_team_by_id;
    Node<player_in_scoreboard>* m_player_in_scoreboard;


    //public:
    /***********************************************************
         * Player: constructor for Player
         * 
         * @param m_player_id the player's unique id
         * @param m_team_id the player's team
         * @param m_games_played number of games this player has played
         * @param m_goals number of goals this player scored
         * @param m_cards number of cards this played has
         * @param m_goalkeeper boolean value if this player can be a goalkeeper
    ***********************************************************/

    player(int player_id, int team_id, int games_played, int goals, int cards, bool goalkeeper) : 
    m_player_id(player_id), m_team_id(team_id), m_games_played(games_played), m_goals(goals), m_cards(cards), 
    m_goalkeeper(goalkeeper), m_player_in_team_by_score(nullptr), m_player_in_team_by_id(nullptr), m_player_in_scoreboard(nullptr)
    {};

    /***********************************************************
    * ~Player: destructor for Player
    ***********************************************************/

    ~player() {
        //not sure if need to delete
        /*
        delete m_player_in_team_by_score;
        delete m_player_in_team_by_id;
        delete m_player_in_scoreboard;
        */
    };

    
};


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
      //  delete m_player;
       // delete m_my_team;
    }

};

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

#endif //PLAYERS_H_
