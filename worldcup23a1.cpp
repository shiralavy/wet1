#include "worldcup23a1.h"
#define ONE 1

int world_cup_t::m_best_player = 0;
int world_cup_t::m_num_players = 0;

int world_cup_t::max(int a, int b)
{
	return (a > b) ? a : b;
}

/****************Begining implementation AVL tree for Teams******************************/
int world_cup_t::calc_height_teams(shared_ptr<Team> node)
{
	int h = 0;
	if (node != nullptr)
	{
		if (node->m_left_team != nullptr && node->m_right_team != nullptr)
		{
			h = 1 + max(node->m_left_team->m_height_teams, node->m_right_team->m_height_teams);
		}
		else if (node->m_left_team && node->m_right_team == nullptr)
		{
			h = 1 + node->m_left_team->m_height_teams;
		}
		else if (node->m_right_team && node->m_left_team == nullptr)
		{
			h = 1 + node->m_right_team->m_height_teams;
		}
	}
	return h;
}

int world_cup_t::calc_BF_teams(shared_ptr<Team> team)
{
	if (!team)
	{
		return 0;
	}
	if (team->m_right_team && team->m_left_team)
	{
		return team->m_left_team->m_height_teams - team->m_right_team->m_height_teams;
	}
	else if (team->m_left_team && team->m_right_team == nullptr)
	{
		return team->m_left_team->m_height_teams;
	}
	else if (team->m_left_team == nullptr && team->m_right_team)
	{
		return -team->m_right_team->m_height_teams;
	}
}

shared_ptr<Team> world_cup_t::find_team_teams(shared_ptr<Team> nodeTeam, int teamID)
{
	if (teamID <= 0)
	{
		return nullptr;
	}
	if (!nodeTeam)
	{
		return nullptr;
	}
	if (nodeTeam->m_team_id == teamID)
	{
		return nodeTeam;
	}
	if (teamID < nodeTeam->m_team_id)
	{
		find_team_teams(nodeTeam->m_left_team, teamID);
	}
	if (teamID > nodeTeam->m_team_id)
	{
		find_team_teams(nodeTeam->m_right_team, teamID);
	}
}

shared_ptr<Team> world_cup_t::insert_team_teams(shared_ptr<Team> nodeTeam, int teamID, int points)
{
	if (nodeTeam == nullptr)
	{
		shared_ptr<Team> root = make_shared<Team>(teamID, points);
		return root;
	}
	if (teamID < nodeTeam->m_team_id)
	{
		nodeTeam->m_left_team = insert_team_teams(nodeTeam->m_left_team, teamID, points);
	}
	if (teamID > nodeTeam->m_team_id)
	{
		nodeTeam->m_right_team = insert_team_teams(nodeTeam->m_right_team, teamID, points);
	}
	nodeTeam->m_height_teams = calc_height_teams(nodeTeam);

	int balance_factor = calc_BF_teams(nodeTeam);
	if (balance_factor == 2 && calc_BF_teams(nodeTeam->m_left_team) == 1)
	{
		// LL ROTATION
	}
	else if (balance_factor == -2 && calc_BF_teams(nodeTeam->m_right_team) == -1)
	{
		// RR ROTATION
	}
	else if (balance_factor == -2 && calc_BF_teams(nodeTeam->m_right_team) == 1)
	{
		// RL ROTATION
	}
	else if (balance_factor == 2 && calc_BF_teams(nodeTeam->m_left_team) == -1)
	{
		// LR ROTATION
	}
}

shared_ptr<Team> world_cup_t::rrrotation_teams(shared_ptr<Team> node)
{
	shared_ptr<Team> p;
	shared_ptr<Team> tp;
	p = node;
	tp = p->m_right_team;

	p->m_right_team = tp->m_left_team;
	tp->m_left_team = p;
	return tp;
}

shared_ptr<Team> world_cup_t::llrotation_teams(shared_ptr<Team> node)
{
	shared_ptr<Team> p;
	shared_ptr<Team> tp;
	p = node;
	tp = p->m_left_team;
	p->m_left_team = tp->m_right_team;
	tp->m_right_team = p;
	return tp;
}

shared_ptr<Team> world_cup_t::rlrotation_teams(shared_ptr<Team> node)
{
	shared_ptr<Team> p;
	shared_ptr<Team> tp;
	shared_ptr<Team> tp2;
	p = node;
	tp = p->m_right_team;
	tp2 = p->m_right_team->m_left_team;
	p->m_right_team = tp2->m_left_team;
	tp->m_left_team = tp2->m_right_team;
	tp2->m_left_team = p;
	tp2->m_right_team = tp;
	return tp2;
}

shared_ptr<Team> world_cup_t::lrrotation_teams(shared_ptr<Team> node)
{
	shared_ptr<Team> p;
	shared_ptr<Team> tp;
	shared_ptr<Team> tp2;
	p = node;
	tp = p->m_left_team;
	tp2 = p->m_left_team->m_right_team;
	p->m_right_team = tp2->m_left_team;
	tp->m_left_team = tp2->m_right_team;
	tp2->m_left_team = p;
	tp2->m_right_team = tp;
	return tp2;
}

shared_ptr<Team> minValueNode(shared_ptr<Team> node) //helps the delete function 
{ 
   shared_ptr<Team> current = node; 
  
    while (current->m_left_team != NULL) 
        current = current->m_left_team; 
  
    return current; 
} 

shared_ptr<Team> deleteNode(shared_ptr<Team> node, int teamId)
{
	if (node == NULL)
	{
		return node;
	}

	if (teamId < node->m_team_id)
	{
		node->m_left_team = deleteNode(node->m_left_team, teamId);
	}

	else if (teamId > node->m_team_id)
	{
		node->m_right_team = deleteNode(node->m_right_team, teamId);
	}
	else
	{
		if ((node->m_left_team == NULL) || (node->m_right_team == NULL))
		{
			shared_ptr<Team> temp = node->m_left_team ? node->m_left_team:node->m_right_team;
            if(temp == NULL)
        {
            temp=node;
            node=NULL;
        }
        else{
            *node=*temp;
        }
		  
	}
    else
    {
       shared_ptr<Team> temp=minValueNode(node->m_right_team);

       node->m_team_id=temp->m_team_id;

       node->m_right_team=deleteNode(node->m_right_team,temp->m_team_id);
    }

 } 

    if(node==NULL)
    {
        return node;
    }
	

    node->m_height_teams=1+ max(node->m_right_team->m_height_teams,node->m_left_team->m_height_teams);
/*
    int balance=calc_BF_teams(node);


    if(balance>ONE &&calc_BF_teams(node->m_left_team)>=0 )
    {
        return llrotation_teams(node);
    }

    if(balance-1 &calc_BF_teams(node->m_right_team)<=0)
    {
        
    }
    
    if(balance> ONE && calc_BF_teams(node->m_left_team)<0)
    {
        node->m_left_team=lrrotation_teams(node->m_left_team);
        return node;
    }

    */

}

/*******************************end of AVL tree for Teams*****************************************/
/****************Begining implementation AVL tree for Players sorted by player ID*****************/
int world_cup_t::calc_BF_players_ID(shared_ptr<Player> player)
{
	if (!player)
	{
		return 0;
	}
	if (player->m_left_general_id && player->m_right_general_id)
	{
		return player->m_left_general_id->m_height_general_id - player->m_right_general_id->m_height_general_id;
	}
	else if (player->m_left_general_id && player->m_right_general_id == nullptr)
	{
		return player->m_left_general_id->m_height_general_id;
	}
	else if (player->m_left_general_id == nullptr && player->m_right_general_id)
	{
		return -player->m_right_general_id->m_height_general_id;
	}
}

int world_cup_t::calc_height_players_ID(shared_ptr<Player> player){
	int h = 0;
	if (player != nullptr)
	{
		if (player->m_left_general_id != nullptr && player->m_right_general_id != nullptr)
		{
			h = 1 + max(player->m_left_general_id->m_height_general_id, player->m_right_general_id->m_height_general_id);
		}
		else if (player->m_left_general_id && player->m_right_general_id == nullptr)
		{
			h = 1 + player->m_left_general_id->m_height_general_id;
		}
		else if (player->m_right_general_id && player->m_left_general_id == nullptr)
		{
			h = 1 + player->m_right_general_id->m_height_general_id;
		}
	}
	return h;
}

shared_ptr<Player> world_cup_t::find_player_ID(shared_ptr<Player> nodePlayer, int playerID)
{
	if (playerID <= 0)
	{
		return nullptr;
	}
	if (!nodePlayer)
	{
		return nullptr;
	}
	if (nodePlayer->m_player_id == playerID)
	{
		return nodePlayer;
	}
	if (playerID < nodePlayer->m_player_id)
	{
		find_player_ID(nodePlayer->m_left_general_id, playerID);
	}
	if (playerID > nodePlayer->m_player_id)
	{
		find_player_ID(nodePlayer->m_right_general_id, playerID);
	}
}

shared_ptr<Player> world_cup_t::insert_player_ID(shared_ptr<Player> nodePlayer, int playerID, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper)
{
	if (nodePlayer == nullptr)
	{
		shared_ptr<Player> root = make_shared<Player>(playerID, teamId,  gamesPlayed,  goals,  cards,  goalKeeper);
		return root;
	}
	if (playerID < nodePlayer->m_player_id)
	{
		nodePlayer->m_left_general_id = insert_player_ID(nodePlayer->m_left_general_id, playerID, teamId,  gamesPlayed,  goals,  cards,  goalKeeper);
	}
	if (playerID > nodePlayer->m_player_id)
	{
		nodePlayer->m_right_general_id = insert_player_ID(nodePlayer->m_right_general_id, playerID,  teamId,  gamesPlayed,  goals,  cards,  goalKeeper);
	}
	nodePlayer->m_height_general_id = calc_height_players_ID(nodePlayer);

	int balance_factor = calc_BF_players_ID(nodePlayer);
	if (balance_factor == 2 && calc_BF_players_ID(nodePlayer->m_left_general_id) == 1)
	{
		// LL ROTATION
	}
	else if (balance_factor == -2 && calc_BF_players_ID(nodePlayer->m_right_general_id) == -1)
	{
		// RR ROTATION
	}
	else if (balance_factor == -2 && calc_BF_players_ID(nodePlayer->m_right_general_id) == 1)
	{
		// RL ROTATION
	}
	else if (balance_factor == 2 && calc_BF_players_ID(nodePlayer->m_left_general_id) == -1)
	{
		// LR ROTATION
	}
}

/****************************end of AVL tree for Players by player ID*************************************/
/****************Begining implementation AVL tree for Players sorted by player SCORE*****************/
int world_cup_t::calc_BF_players_scores(shared_ptr<Player> player){
	if (!player)
	{
		return 0;
	}
	if (player->m_left_general_score && player->m_right_general_score)
	{
		return player->m_left_general_score->m_height_general_score - player->m_right_general_score->m_height_general_score;
	}
	else if (player->m_left_general_score && player->m_right_general_score == nullptr)
	{
		return player->m_left_general_score->m_height_general_score;
	}
	else if (player->m_left_general_score == nullptr && player->m_right_general_score)
	{
		return -player->m_right_general_score->m_height_general_score;
	}
}

int world_cup_t::calc_height_players_score(shared_ptr<Player> player)
{
	int h = 0;
	if (player != nullptr)
	{
		if (player->m_left_general_score != nullptr && player->m_right_general_score != nullptr)
		{
			h = 1 + max(player->m_left_general_score->m_height_general_score, player->m_right_general_score->m_height_general_score);
		}
		else if (player->m_left_general_score && player->m_right_general_score == nullptr)
		{
			h = 1 + player->m_left_general_score->m_height_general_score;
		}
		else if (player->m_right_general_score && player->m_left_general_score == nullptr)
		{
			h = 1 + player->m_right_general_score->m_height_general_score;
		}
	}
	return h;
}

shared_ptr<Player> world_cup_t::find_player_score(shared_ptr<Player> nodePlayer, int playerScore)
{
	if (playerScore <= 0)
	{
		return nullptr;
	}
	if (!nodePlayer)
	{
		return nullptr;
	}
	if (nodePlayer->m_player_score == playerScore)
	{
		return nodePlayer;
	}
	if (playerScore < nodePlayer->m_player_score)
	{
		find_player_ID(nodePlayer->m_left_general_score, playerScore);
	}
	if (playerScore > nodePlayer->m_player_score)
	{
		find_player_ID(nodePlayer->m_right_general_score, playerScore);
	}
}
shared_ptr<Player> world_cup_t::insert_player_score(shared_ptr<Player> nodePlayer, int playerScore){
	//TODO
}

/****************************end of AVL tree for Players by player SCORE*************************************/

world_cup_t::world_cup_t()
{
	// TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId, int points)
{
	if (teamId <= 0 || points < 0)
	{
		return StatusType::INVALID_INPUT;
	}
	if (!this->m_root_teams)
	{
		this->m_root_teams = insert_team(this->m_root_teams, teamId, points);
		if (!this->m_root_teams)
		{
			return StatusType::ALLOCATION_ERROR;
		}
		else
		{
			return StatusType::SUCCESS;
		}
	}
	else
	{
		if (find_team(this->m_root_teams, teamId) != nullptr)
		{
			return StatusType::FAILURE;
		}
	}

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	// TODO: Your code goes here
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
								   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
											int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
	static int i = 0;
	return (i++ == 0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
	output[0] = 4001;
	output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}
