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


    
    
};

#endif //PLAYERS_IN_TEAM_H_
