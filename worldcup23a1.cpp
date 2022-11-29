#include "worldcup23a1.h"
#define ONE 1

world_cup_t::world_cup_t() : m_best_player(-1), m_num_players(0)
{
	try
	{
		m_tree_players_by_id = make_shared<AVLtree<player>>();
	}
	catch (std::bad_alloc &)
	{
		throw;
	}
	try
	{
		m_tree_players_by_score = make_shared<AVLtree<player_in_scoreboard>>();
	}
	catch (std::bad_alloc &)
	{
		// need to delete m_tree_players_by_id - will the d'tor be called automatically because of the shared_ptr?
		throw;
	}
	try
	{
		m_tree_teams_by_id = make_shared<AVLtree<Team>>();
	}
	catch (std::bad_alloc &)
	{
		// need to delete m_tree_players_by_id and m_tree_players_by_score?
		throw;
	}
	try
	{
		m_tree_ready_teams = make_shared<AVLtree<ready_team>>();
	}
	catch (std::bad_alloc &)
	{
		// need to delete m_tree_players_by_id and m_tree_players_by_score and m_tree_teams_by_id?
		throw;
	}
}

//will need to delete ptr's since we are not using shared ptr 
world_cup_t::~world_cup_t()
{

}

StatusType world_cup_t::add_team(int teamId, int points)
{
	if (teamId <= 0 || points < 0)
	{
		// invalid teamId or points values
		return StatusType::INVALID_INPUT;
	}
	if (this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId) != nullptr)
	{
		// there is already a team with this ID in the tournament
		return StatusType::FAILURE;
	}
	else
	{
		try
		{
			Team newTeam(teamId,points);
			//Node<Team>* newTeamNode = new Node<Team>(newTeam, teamId);
			Node<Team>* root = this->m_tree_teams_by_id->m_root;
			this->m_tree_teams_by_id->insertNode(root, teamId, newTeam);
		}
		catch (std::bad_alloc &)
		{
			return StatusType::ALLOCATION_ERROR;
		}
	}

	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if (teamId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<Team>* TeamToRemove = (this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId));
	
	//this checks if the team does not exsist and if the team is empty 
	if (TeamToRemove == nullptr || TeamToRemove->m_element->m_num_players != 0)
	{
		return StatusType::FAILURE;
	}

	//to be cotinued 

	
	
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
								   int goals, int cards, bool goalKeeper)
{
	if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 ||
		(gamesPlayed == 0 && (goals > 0 || cards > 0)))
	{
		return StatusType::INVALID_INPUT;
	}
	if (this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId) != nullptr ||
		this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId) == nullptr)
	{
		// there is already a player with this ID in the tournament or there is no team in the tournament with this ID
		return StatusType::FAILURE;
	}
	else
	{ // the player does not exist, the team does exist
		try
		{
			//insert player into AVL TREE with players sorted by id
			player newPlayer(playerId, teamId, gamesPlayed, goals, cards, goalKeeper);
			Node<player>* root = this->m_tree_players_by_id->m_root;
			this->m_tree_players_by_id->insertNode(root, playerId, newPlayer);
			
			//insert player into AVL TREE of players inside the appropriate team
			Node<player>* insertedPlayer = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId);
			Node<Team>* teamForNewPlayer = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId);
			player_in_team newPlayerInTeam(playerId, teamId, gamesPlayed, goals, cards, goalKeeper, insertedPlayer, teamForNewPlayer);
			teamForNewPlayer->m_element.m_tree_players_in_team->insertNode(teamForNewPlayer->m_element.m_tree_players_in_team->m_root, playerId, newPlayerInTeam);

			//update the pointer of player in big AVL TREE to its location in the team
			insertedPlayer->m_element.m_player_in_team = teamForNewPlayer->m_element.m_tree_players_in_team->findNode(teamForNewPlayer->m_element.m_tree_players_in_team->m_root, playerId);
			
			//if after the insertion of the player this team has now become ready, insert this team into the AVL TREE with ready teams
			bool teamWasReady = teamForNewPlayer->m_element.check_team_ready();
			teamForNewPlayer->m_element.m_num_players++;
			if (goalKeeper)
			{
				teamForNewPlayer->m_element.m_num_goalkeeprs++;
			}
			if (!teamWasReady && teamForNewPlayer->m_element.check_team_ready())
			{
				ready_team newReadyTeam(teamForNewPlayer->m_key, teamForNewPlayer);
				this->m_tree_ready_teams->insertNode(this->m_tree_ready_teams->m_root, teamId, newReadyTeam);
			}

			//insert player into AVL TREE containing players by score
			//code code code 

		}
		catch (std::bad_alloc &)
		{
			return StatusType::ALLOCATION_ERROR;
		}
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if (playerId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	shared_ptr<Node<player>> tempPlayer = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId);

	if (tempPlayer == nullptr)
	{
		return StatusType::FAILURE;
	}

	Node<player_in_team>* temp_player_in_team = tempPlayer->m_element->m_player_in_team;
	Node<Team>* temp_team_containing_player = temp_player_in_team->m_element->m_my_team;
	if (temp_team_containing_player->m_element->check_team_ready())
	{
		// dont forget to relese the pointer
		//update num_players in the team and num goalkeepers
		temp_player_in_team->m_element->m_my_team->m_element->m_num_players--;
		// checks if the player to remove is a goalkeeper
		if (temp_player_in_team->m_element->m_goalkeeper)
		{
			temp_player_in_team->m_element->m_my_team->m_element->m_num_goalkeeprs--;
		}
		if (!temp_team_containing_player->m_element->check_team_ready())
		{
			//remove from tree of ready teams


			/*Node<ready_team>* team_ready_team
			temp_player_in_team->m_element->m_my_team->m_element->m_tree_players_in_team
				->deleteNode(temp_player_in_team->m_element->m_my_team->m_element->m_tree_players_in_team->m_root, playerId);
		*/
		}
	}

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
