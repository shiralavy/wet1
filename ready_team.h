#ifndef READY_TEAM_H_
#define READY_TEAM_H_

#include <stdexcept>
#include "AVLtree.h"
#include "team.h"

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

    ready_team(int team_id, Node<Team>* team) : m_team_id(team_id){
        m_team = new Node<Team>(team_id);
    };

    /***********************************************************
    * ~ready_team: destructor for ready_team
    ***********************************************************/

    ~ready_team(){
        delete m_team;
    };




};

#endif //READY_TEAM_H_
