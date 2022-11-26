#ifndef PLAYERS_IN_SCOREBOARD_H_
#define PLAYERS_IN_SCOREBOARD_H_

#include <stdexcept>
#include <memory>
#include "player.h"
#include "AVLtree.h"


using namespace std;

class player_in_scoreboard {
    public:
    int m_player_id;
    int m_team_id;
    int m_games_played;
    int m_goals;
    int m_cards;
    bool m_goalkeeper;
    shared_ptr<Node<player>> m_player;
    shared_ptr<Node<player>> m_prev_player_by_score; //inside linked list sorted by player score
    shared_ptr<Node<player>> m_next_player_by_score; //inside linked list sorted by player score

};

#endif //PLAYERS_IN_SCOREBOARD_H_