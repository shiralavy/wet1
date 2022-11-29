#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "player_in_team.h"
#include "player_in_scoreboard.h"

using namespace std;

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
    Node<player_in_team>* m_player_in_team;
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
    m_player_id(player_id), m_team_id(team_id), m_games_played(games_played), m_goals(goals), m_cards(cards), m_goalkeeper(goalkeeper)
    {
        m_player_in_team = new Node<player_in_team>(player_id);
        m_player_in_scoreboard = new Node<player_in_scoreboard>(player_id);

    };

    /***********************************************************
    * ~Player: destructor for Player
    ***********************************************************/

    ~player() {
        delete m_player_in_team;
        delete m_player_in_scoreboard;
    };

    
};

#endif //PLAYERS_H_
