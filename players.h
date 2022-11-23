#ifndef PLAYERS_H_
#define PLAYERS_H_

#include <stdexcept>
#include <memory>

using namespace std;

class Player {
    //private:
    int m_player_id;
    int m_team_id;
    int m_games_played;
    int m_goals;
    int m_cards;
    int m_closest_player;
    bool m_goalkeeper;

    int m_height_general_id;
    int m_height_general_score;
    int m_height_team_id; 

    //for AVL tree containing all players sorted by player ID:
    shared_ptr<Player> m_left_general_id; 
    shared_ptr<Player> m_right_general_id; 
    //for AVL tree containing all players sorted by player score:
    shared_ptr<Player> m_left_general_score; 
    shared_ptr<Player> m_right_general_score; 
    //for AVL tree inside a specific team containing all players in that team sorted by player ID:
    shared_ptr<Player> m_left_team_id; 
    shared_ptr<Player> m_right_team_id; 
    //for linked list containing all players sorted by score
    shared_ptr<Player> m_next_list_scores; 
    shared_ptr<Player> m_prev_list_scores; 

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

    Player(int player_id, int team_id, int games_played, int goals, int cards, bool goalkeeper) : 
    m_player_id(player_id), m_team_id(team_id), m_games_played(games_played), m_goals(goals), m_cards(cards), m_goalkeeper(goalkeeper), m_closest_player(0),
    m_height_general_id(0), m_height_general_score(0), m_height_team_id(0)
    /*m_left_general_id(nullptr), m_right_general_id(nullptr), m_left_general_score(nullptr), m_right_general_score(nullptr), 
    m_left_team_id(nullptr), m_right_team_id(nullptr) */
    {};

    /***********************************************************
    * ~Player: destructor for Player
    ***********************************************************/

    ~Player() = default;

    
};

#endif //PLAYERS_H_
