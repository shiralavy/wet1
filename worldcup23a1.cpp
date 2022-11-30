#include "worldcup23a1.h"
#define ONE 1

world_cup_t::world_cup_t() : m_best_player(-1), m_num_players(0)
{
	try
	{
		m_tree_players_by_id = new AVLtree<player>();
	}
	catch (std::bad_alloc &)
	{
		throw;
	}
	try
	{
		m_tree_players_by_score = new AVLtree<player_in_scoreboard>();
	}
	catch (std::bad_alloc &)
	{
		delete m_tree_players_by_id;
		throw;
	}
	try
	{
		m_tree_teams_by_id = new AVLtree<Team>();
	}
	catch (std::bad_alloc &)
	{
		delete m_tree_players_by_id;
		delete m_tree_players_by_score;
		throw;
	}
	try
	{
		m_tree_ready_teams = new AVLtree<ready_team>();
	}
	catch (std::bad_alloc &)
	{
		delete m_tree_players_by_id;
		delete m_tree_players_by_score;
		delete m_tree_teams_by_id;
		throw;
	}
}

// will need to delete ptr's since we are not using shared ptr
world_cup_t::~world_cup_t()
{
	// code code code
}

StatusType world_cup_t::add_team(int teamId, int points)
{
	if (teamId <= 0 || points < 0)
	{
		// invalid teamId or points values
		return StatusType::INVALID_INPUT;
	}
	if (this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0) != nullptr)
	{
		// there is already a team with this ID in the tournament
		return StatusType::FAILURE;
	}
	else
	{
		try
		{
			Team newTeam(teamId, points);
			// Node<Team>* newTeamNode = new Node<Team>(newTeam, teamId);
			Node<Team> *root = this->m_tree_teams_by_id->m_root;
			this->m_tree_teams_by_id->insertNode(root, teamId, 0, 0, newTeam);
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
	Node<Team> *TeamToRemove = (this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0));

	// this checks if the team does not exsist or if the team is empty
	if (TeamToRemove == nullptr || TeamToRemove->m_element.m_num_players != 0)
	{
		return StatusType::FAILURE;
	}

	// we deleted the team from the id tree
	this->m_tree_teams_by_id->deleteNode(TeamToRemove, teamId, 0, 0);
	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
								   int goals, int cards, bool goalKeeper)
{
	if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 ||
		(gamesPlayed == 0 && (goals > 0 || cards > 0)))
	{
		return StatusType::INVALID_INPUT;
	}
	if (this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0) != nullptr ||
		this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0) == nullptr)
	{
		// there is already a player with this ID in the tournament or there is no team in the tournament with this ID
		return StatusType::FAILURE;
	}
	else
	{ // the player does not exist, the team does exist
		try
		{
			// insert player into AVL TREE with players sorted by id
			player newPlayer(playerId, teamId, gamesPlayed, goals, cards, goalKeeper);
			Node<player> *root = this->m_tree_players_by_id->m_root;
			this->m_tree_players_by_id->insertNode(root, playerId, 0, 0, newPlayer);

			// insert player into AVL TREE of players inside the appropriate team sorted by score
			Node<player> *insertedPlayer = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);
			Node<Team> *teamForNewPlayer = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
			player_in_team newPlayerInTeamByScore(playerId, teamId, gamesPlayed, goals, cards, goalKeeper, insertedPlayer, teamForNewPlayer);
			teamForNewPlayer->m_element.m_tree_players_in_team_by_score->insertNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_score->m_root, goals, cards, playerId, newPlayerInTeamByScore);
			teamForNewPlayer->m_element.m_winning_num = teamForNewPlayer->m_element.m_winning_num + goals - cards;

			// insert player into AVL TREE of players inside the appropriate team sorted by id
			player_in_team newPlayerInTeamByID(playerId, teamId, gamesPlayed, goals, cards, goalKeeper, insertedPlayer, teamForNewPlayer);
			teamForNewPlayer->m_element.m_tree_players_in_team_by_score->insertNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_score->m_root, playerId, 0, 0, newPlayerInTeamByID);

			// update the pointers of player in big AVL TREE to its locations in the team
			insertedPlayer->m_element.m_player_in_team_by_score = teamForNewPlayer->m_element.m_tree_players_in_team_by_score->findNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_score->m_root, goals, cards, playerId);
			insertedPlayer->m_element.m_player_in_team_by_id = teamForNewPlayer->m_element.m_tree_players_in_team_by_id->findNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_score->m_root, playerId, 0, 0);

			// if after the insertion of the player this team has now become ready, insert this team into the AVL TREE with ready teams
			bool teamWasReady = teamForNewPlayer->m_element.check_team_ready();
			teamForNewPlayer->m_element.m_num_players++;
			if (goalKeeper)
			{
				teamForNewPlayer->m_element.m_num_goalkeeprs++;
			}
			if (!teamWasReady && teamForNewPlayer->m_element.check_team_ready())
			{
				ready_team newReadyTeam(teamForNewPlayer->m_key1, teamForNewPlayer);
				this->m_tree_ready_teams->insertNode(this->m_tree_ready_teams->m_root, teamId, 0, 0, newReadyTeam);
			}

			// update the team's winning number because this player is added
			int num = goals - cards;
			teamForNewPlayer->m_element.m_winning_num += num;

			// insert player into AVL TREE containing players by score
			player_in_scoreboard newPlayerByScore(playerId);
			this->m_tree_players_by_score->insertNode(this->m_tree_players_by_score->m_root, goals, cards, playerId, newPlayerByScore);
			insertedPlayer->m_element.m_player_in_scoreboard = this->m_tree_players_by_score->findNode(this->m_tree_players_by_score->m_root, goals, cards, playerId);

		}
		catch (std::bad_alloc &)
		{
			return StatusType::ALLOCATION_ERROR;
		}
	}
	this->m_best_player = this->m_tree_players_by_score->m_heighest_key3;
	this->m_num_players++;
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if (playerId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<player> *playerToRemove = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);

	if (playerToRemove == nullptr)
	{
		return StatusType::FAILURE;
	}

	// delete from tree of all players by score
	this->m_tree_players_by_score->deleteNode(this->m_tree_players_by_score->m_root, playerToRemove->m_element.m_goals,
											  playerToRemove->m_element.m_cards, playerToRemove->m_element.m_player_id);


	//if because of removing this player a team that was ready has now become NOT ready, we remove this team:
	Node<player_in_team> *tempPlayerInTeamByScore = playerToRemove->m_element.m_player_in_team_by_score;
	Node<Team> *tempTeamContainingPlayer = tempPlayerInTeamByScore->m_element.m_my_team;
	if (tempTeamContainingPlayer->m_element.check_team_ready())
	{

		// update num_players in the team and num goalkeepers
		tempTeamContainingPlayer->m_element.m_num_players--;
		// checks if the player to remove is a goalkeeper
		if (tempPlayerInTeamByScore->m_element.m_goalkeeper)
		{
			tempTeamContainingPlayer->m_element.m_num_goalkeeprs--;
		}
		// remove from tree ready teams
		if (!tempTeamContainingPlayer->m_element.check_team_ready())
		{
			this->m_tree_ready_teams->deleteNode(this->m_tree_ready_teams->m_root, playerToRemove->m_element.m_team_id, 0, 0);
		}
	}
	// update the team's winning number because this player is removed
	int num = (playerToRemove->m_element.m_goals)-(playerToRemove->m_element.m_cards);
	tempTeamContainingPlayer->m_element.m_winning_num -= num;


	//delete from tree of players by score inside Team
	tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_score->deleteNode(tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_score->m_root, playerToRemove->m_element.m_goals, playerToRemove->m_element.m_cards, playerToRemove->m_element.m_player_id);

	//delete from tree of players by id inside team
	tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_id->deleteNode(tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_id->m_root, playerToRemove->m_element.m_player_id, 0,0);
	
	// delete from tree of all players by playerId
	this->m_tree_players_by_id->deleteNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);

	this->m_best_player = this->m_tree_players_by_score->m_heighest_key3;
	this->m_num_players--;
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
											int scoredGoals, int cardsReceived)
{
	if (playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0)
	{
		return StatusType::INVALID_INPUT;
	}

	Node<player> *playerToUpdate = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);

	// player does not exists
	if (!playerToUpdate)
	{
		return StatusType::FAILURE;
	}
	playerToUpdate->m_element.m_games_played += gamesPlayed;
	playerToUpdate->m_element.m_goals += scoredGoals;
	playerToUpdate->m_element.m_cards += cardsReceived;

	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	Node<Team> *team1 = this->m_tree_teams_by_id->findNode(m_tree_teams_by_id->m_root, teamId1, 0, 0);
	Node<Team> *team2 = this->m_tree_teams_by_id->findNode(m_tree_teams_by_id->m_root, teamId2, 0, 0);

	// both teams are ready
	if (team1->m_element.check_team_ready() && team2->m_element.check_team_ready())
	{
		int powerNum1 = team1->m_element.m_winning_num;
		int powerNum2 = team2->m_element.m_winning_num;
		// tie
		if (powerNum1 == powerNum2)
		{
			team1->m_element.m_points += 1;
			team2->m_element.m_points += 1;
			return StatusType::SUCCESS;
		}
		// some team won
		else if (powerNum1 > powerNum2)
		{
			team1->m_element.m_points += 3;
			return StatusType::SUCCESS;
		}
		else
		{
			team2->m_element.m_points += 3;
			return StatusType::SUCCESS;
		}
	}
	return StatusType::FAILURE;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	if (playerId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<player> *playerToBringGames = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);

	if (playerToBringGames == nullptr)
	{
		return StatusType::FAILURE;
	}

	return playerToBringGames->m_element.m_games_played;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if (teamId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<Team> *currentTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
	if (!currentTeam)
	{
		return StatusType::FAILURE;
	}
	return currentTeam->m_element.m_points;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	if (newTeamId <= 0 || teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
	{
		return StatusType::INVALID_INPUT;
	}

	Node<Team> *team1 = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId1, 0, 0);
	Node<Team> *team2 = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId2, 0, 0);
	Node<Team> *newTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId2, 0, 0);
	// check if one or two of the teams dont excise
	/*
	ask about the Failure option
	*/
	if (!team1 || !team2)
	{
		return StatusType::FAILURE;
	}
	//
	if (newTeam)
	{
		if (newTeam->m_element.m_team_id != teamId1 && newTeam->m_element.m_team_id != teamId2)
		{
			return StatusType::FAILURE;
		}
	}
	// we will add team2 to team1 and delete team2
	if (newTeamId == teamId1)
	{
	}

	this->add_team(newTeamId, team1->m_element.m_points + team2->m_element.m_points);

	for (int i = 0; i < team1->m_element.m_num_players; i++)
	{
	}
	// add all players from team1 add_player()
	// add all players team 2
	// delete team 1 remove_team()
	// delete team2 remove_team()

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	if (teamId == 0)
	{
		return StatusType::INVALID_INPUT;
	}
	if (teamId < 0)
	{
		if (this->m_num_players == 0)
		{
			// no players in the tournament
			return StatusType::FAILURE;
		}
		// returning best player in the tournament
		return this->m_tree_players_by_score->m_heighest_key3;
	}
	else
	{
		Node<Team> *currentTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
		if (!currentTeam || currentTeam->m_element.m_num_players == 0)
		{
			// no team with this id found or no players in this team
			return StatusType::FAILURE;
		}
		// returning best player in the team
		return currentTeam->m_element.m_tree_players_in_team_by_score->m_heighest_key3;
	}
	return StatusType::FAILURE;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	if (teamId == 0)
	{
		return StatusType::INVALID_INPUT;
	}
	else if (teamId < 0)
	{
		// returning number of players in the tournament
		return this->m_num_players;
	}
	else
	{
		Node<Team> *currentTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
		if (!currentTeam)
		{
			// team does not exist in the tournament
			return StatusType::FAILURE;
		}
		// returning number of players in the team
		return currentTeam->m_element.m_num_players;
	}
	static int i = 0;
	return (i++ == 0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	if (teamId == 0)
	{
		return StatusType::INVALID_INPUT;
	}
	else if (teamId < 0)
	{
		// saving all of the players in the array output
		this->m_tree_players_by_score->inOrderVisit(this->m_tree_players_by_score->m_root, output, 0);
	}
	else
	{
		Node<Team> *currentTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
		if (!currentTeam)
		{
			// team does not exist in the tournament
			return StatusType::FAILURE;
		}
		// saving all of the players in the array output
		currentTeam->m_element.m_tree_players_in_team_by_score->inOrderVisit(currentTeam->m_element.m_tree_players_in_team_by_score->m_root, output, 0);
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	if (playerId <= 0 || teamId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<Team> *currentTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
	Node<player_in_team> *playerInTeam = currentTeam->m_element.m_tree_players_in_team_by_score->findNode(currentTeam->m_element.m_tree_players_in_team_by_score->m_root, , , playerId) return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	if (minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<ready_team> *currTeam = this->m_tree_ready_teams->m_root;
	
	while (currTeam)
	{
		// good teams
		if (currTeam->m_element.m_team_id <= maxTeamId && currTeam->m_element.m_team_id >= minTeamId)
		{
		}
	}
	return 2;
}
