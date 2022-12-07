#include "worldcup23a1.h"
#define ONE 1

/****************************private functions**********************************/

// this function is similar to the regular add_player, but in this case the time complexity is logn instead of
// logn+logk because we dont need to find the team in  the tree of teams and we dont insert the team to the ready_teams tree if in now bacame ready
StatusType world_cup_t::add_player(int playerId, Node<Team> *team, int gamesPlayed,
								   int goals, int cards, bool goalKeeper)
{

	if (playerId <= 0 || !team || gamesPlayed < 0 || goals < 0 ||
		(gamesPlayed == 0 && (goals > 0 || cards > 0)))
	{
		return StatusType::INVALID_INPUT;
	}
	if (this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0) != nullptr)
	{
		// there is already a player with this ID in the tournament
		return StatusType::FAILURE;
	}
	else
	{ // the player does not exist, the team does exist
		try
		{
			// insert player into AVL TREE with players sorted by id
			player newPlayer(playerId, team->m_element.m_team_id, gamesPlayed, goals, cards, goalKeeper);
			Node<player> *root = this->m_tree_players_by_id->m_root;
			this->m_tree_players_by_id->insertNode(root, playerId, 0, 0, newPlayer);

			// insert player into AVL TREE of players inside the appropriate team sorted by score
			Node<player> *insertedPlayer = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);
			player_in_team newPlayerInTeamByScore(playerId, team->m_element.m_team_id,  insertedPlayer, team);
			team->m_element.m_tree_players_in_team_by_score->insertNode(team->m_element.m_tree_players_in_team_by_score->m_root, goals, cards, playerId, newPlayerInTeamByScore);
			team->m_element.m_winning_num = team->m_element.m_winning_num + goals - cards;
			/*
			   player_in_team(int player_id, int team_id, Node<player>* player = nullptr, Node<Team>* team = nullptr) : 
  				  m_player_id(player_id), m_team_id(team_id), m_player(player), m_my_team(team)
    {};*/

			// insert player into AVL TREE of players inside the appropriate team sorted by id
			player_in_team newPlayerInTeamByID(playerId, team->m_element.m_team_id, insertedPlayer, team);
			team->m_element.m_tree_players_in_team_by_score->insertNode(team->m_element.m_tree_players_in_team_by_score->m_root, playerId, 0, 0, newPlayerInTeamByID);

			// update the pointers of player in big AVL TREE to its locations in the team
			insertedPlayer->m_element.m_player_in_team_by_score = team->m_element.m_tree_players_in_team_by_score->findNode(team->m_element.m_tree_players_in_team_by_score->m_root, goals, cards, playerId);
			insertedPlayer->m_element.m_player_in_team_by_id = team->m_element.m_tree_players_in_team_by_id->findNode(team->m_element.m_tree_players_in_team_by_score->m_root, playerId, 0, 0);

			// update the team's winning number because this player is added
			int num = goals - cards;
			team->m_element.m_winning_num += num;

			// insert player into AVL TREE containing players by score
			player_in_scoreboard newPlayerByScore(playerId);
			this->m_tree_players_by_score->insertNode(this->m_tree_players_by_score->m_root, goals, cards, playerId, newPlayerByScore);
			Node<player_in_scoreboard> *insertedPlayerByScore = this->m_tree_players_by_score->findNode(this->m_tree_players_by_score->m_root, goals, cards, playerId);
			insertedPlayer->m_element.m_player_in_scoreboard = insertedPlayerByScore;

			// update next and prev players by score
			insertedPlayerByScore->m_element.m_next_player_by_score = this->m_tree_players_by_score->closestHigherNode(insertedPlayerByScore);
			insertedPlayerByScore->m_element.m_prev_player_by_score = this->m_tree_players_by_score->closestLowerNode(insertedPlayerByScore);

			// update the next and prev for the players that were affected
            if (insertedPlayerByScore->m_element.m_next_player_by_score) {
                insertedPlayerByScore->m_element.m_next_player_by_score->m_element.m_prev_player_by_score = insertedPlayerByScore;
            }
            if (insertedPlayerByScore->m_element.m_prev_player_by_score){
                insertedPlayerByScore->m_element.m_prev_player_by_score->m_element.m_next_player_by_score = insertedPlayerByScore;
            }
        }
		catch (std::bad_alloc &)
		{
			return StatusType::ALLOCATION_ERROR;
		}
	}
	this->m_best_player = this->m_tree_players_by_score->m_highest_key3;
	this->m_num_players++;
	return StatusType::SUCCESS;
}

// this function is similar to the regular remove player, except it doesnt remove the team from the tree of ready teams in case the team has now become not ready
StatusType world_cup_t::remove_player_except_from_tree_ready_teams(int playerId)
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
	// first we update the next and prev values of the players that pointed to this player
	Node<player_in_scoreboard> *playerToRemoveByScore = playerToRemove->m_element.m_player_in_scoreboard;
	Node<player_in_scoreboard> *nextPlayer = playerToRemoveByScore->m_element.m_next_player_by_score;
	Node<player_in_scoreboard> *prevPlayer = playerToRemoveByScore->m_element.m_prev_player_by_score;
	prevPlayer->m_element.m_next_player_by_score = nextPlayer;
	nextPlayer->m_element.m_prev_player_by_score = prevPlayer;

	// now we delete from the tree
	this->m_tree_players_by_score->deleteNode(this->m_tree_players_by_score->m_root, playerToRemove->m_element.m_goals,
											  playerToRemove->m_element.m_cards, playerToRemove->m_element.m_player_id);

	Node<player_in_team> *tempPlayerInTeamByScore = playerToRemove->m_element.m_player_in_team_by_score;
	Node<Team> *tempTeamContainingPlayer = tempPlayerInTeamByScore->m_element.m_my_team;

	// update the team's winning number because this player is removed
	int num = (playerToRemove->m_element.m_goals) - (playerToRemove->m_element.m_cards);
	tempTeamContainingPlayer->m_element.m_winning_num -= num;

	// delete from tree of players by score inside Team
	tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_score->deleteNode(tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_score->m_root, playerToRemove->m_element.m_goals, playerToRemove->m_element.m_cards, playerToRemove->m_element.m_player_id);

	// delete from tree of players by id inside team
	tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_id->deleteNode(tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_id->m_root, playerToRemove->m_element.m_player_id, 0, 0);

	// delete from tree of all players by playerId
	this->m_tree_players_by_id->deleteNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);

	this->m_best_player = this->m_tree_players_by_score->m_highest_key3;
	this->m_num_players--;
	return StatusType::SUCCESS;
}

/***********************end of private functions**********************************************/

world_cup_t::world_cup_t() : m_best_player(-1), m_num_players(0), m_num_good_teams(0)
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

world_cup_t::~world_cup_t()
{
	delete m_tree_players_by_id;
	delete m_tree_players_by_score;
	delete m_tree_teams_by_id;
	delete m_tree_ready_teams;
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
			this->m_tree_teams_by_id->m_root = this->m_tree_teams_by_id->insertNode(root, teamId, 0, 0, newTeam);
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
    this->m_tree_teams_by_id->m_root = this->m_tree_teams_by_id->deleteNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
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

	Node<Team> *teamForNewPlayer = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
	if (this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0) != nullptr ||
		teamForNewPlayer == nullptr)
	{
		// there is already a player with this ID in the tournament or there is no team in the tournament with this ID
		return StatusType::FAILURE;
	}
	else
	{ // the player does not exist, the team does exist
		try
		{
			// insert player into AVL TREE with players sorted by id
			player newPlayer(playerId, teamId, gamesPlayed - teamForNewPlayer->m_element.m_games_played_by_team, goals, cards, goalKeeper);
			Node<player> *root = this->m_tree_players_by_id->m_root;
			this->m_tree_players_by_id->m_root = this->m_tree_players_by_id->insertNode(root, playerId, 0, 0, newPlayer);

			// insert player into AVL TREE of players inside the appropriate team sorted by score
			Node<player> *insertedPlayer = this->m_tree_players_by_id->findNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);
			player_in_team newPlayerInTeamByScore(playerId, teamId, insertedPlayer, teamForNewPlayer);
            teamForNewPlayer->m_element.m_tree_players_in_team_by_score->m_root = teamForNewPlayer->m_element.m_tree_players_in_team_by_score->insertNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_score->m_root, goals, cards, playerId, newPlayerInTeamByScore);
			teamForNewPlayer->m_element.m_winning_num = teamForNewPlayer->m_element.m_winning_num + goals - cards;

			// insert player into AVL TREE of players inside the appropriate team sorted by id
			player_in_team newPlayerInTeamByID(playerId, teamId, insertedPlayer, teamForNewPlayer);
            teamForNewPlayer->m_element.m_tree_players_in_team_by_id->m_root = teamForNewPlayer->m_element.m_tree_players_in_team_by_id->insertNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_id->m_root, playerId, 0, 0, newPlayerInTeamByID);

			// update the pointers of player in big AVL TREE to its locations in the team
			insertedPlayer->m_element.m_player_in_team_by_score = teamForNewPlayer->m_element.m_tree_players_in_team_by_score->findNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_score->m_root, goals, cards, playerId);
			insertedPlayer->m_element.m_player_in_team_by_id = teamForNewPlayer->m_element.m_tree_players_in_team_by_id->findNode(teamForNewPlayer->m_element.m_tree_players_in_team_by_id->m_root, playerId, 0, 0);

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
                this->m_tree_ready_teams->m_root = this->m_tree_ready_teams->insertNode(this->m_tree_ready_teams->m_root, teamId, 0, 0, newReadyTeam);
				this->m_num_good_teams++;
			}

			// update the team's winning number because this player is added
			int num = goals - cards;
			teamForNewPlayer->m_element.m_winning_num += num;

			// insert player into AVL TREE containing players by score
			player_in_scoreboard newPlayerByScore(playerId);
            this->m_tree_players_by_score->m_root = this->m_tree_players_by_score->insertNode(this->m_tree_players_by_score->m_root, goals, cards, playerId, newPlayerByScore);
			Node<player_in_scoreboard> *insertedPlayerByScore = this->m_tree_players_by_score->findNode(this->m_tree_players_by_score->m_root, goals, cards, playerId);
			insertedPlayer->m_element.m_player_in_scoreboard = insertedPlayerByScore;

			// update next and prev players by score
			insertedPlayerByScore->m_element.m_next_player_by_score = this->m_tree_players_by_score->closestHigherNode(insertedPlayerByScore);
			insertedPlayerByScore->m_element.m_prev_player_by_score = this->m_tree_players_by_score->closestLowerNode(insertedPlayerByScore);

			// update the next and prev for the players that were affected
            if (insertedPlayerByScore->m_element.m_next_player_by_score) {
                insertedPlayerByScore->m_element.m_next_player_by_score->m_element.m_prev_player_by_score = insertedPlayerByScore;
            }
            if (insertedPlayerByScore->m_element.m_prev_player_by_score){
            insertedPlayerByScore->m_element.m_prev_player_by_score->m_element.m_next_player_by_score = insertedPlayerByScore;
		}
        }

		catch (std::bad_alloc &)
		{
			return StatusType::ALLOCATION_ERROR;
		}
	}
	this->m_best_player = this->m_tree_players_by_score->m_highest_key3;
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
	// first we update the next and prev values of the players that pointed to this player
	Node<player_in_scoreboard> *playerToRemoveByScore = playerToRemove->m_element.m_player_in_scoreboard;
	Node<player_in_scoreboard> *nextPlayer = playerToRemoveByScore->m_element.m_next_player_by_score;
	Node<player_in_scoreboard> *prevPlayer = playerToRemoveByScore->m_element.m_next_player_by_score;
	prevPlayer->m_element.m_next_player_by_score = nextPlayer;
	nextPlayer->m_element.m_prev_player_by_score = prevPlayer;

	// now we delete from the tree
	this->m_tree_players_by_score->deleteNode(this->m_tree_players_by_score->m_root, playerToRemove->m_element.m_goals,
											  playerToRemove->m_element.m_cards, playerToRemove->m_element.m_player_id);

	// if because of removing this player a team that was ready has now become NOT ready, we remove this team:
	Node<player_in_team> *tempPlayerInTeamByScore = playerToRemove->m_element.m_player_in_team_by_score;
	Node<Team> *tempTeamContainingPlayer = tempPlayerInTeamByScore->m_element.m_my_team;
	if (tempTeamContainingPlayer->m_element.check_team_ready())
	{

		// update num_players in the team and num goalkeepers
		tempTeamContainingPlayer->m_element.m_num_players--;
		// checks if the player to remove is a goalkeeper
		if (tempPlayerInTeamByScore->m_element.m_player->m_element.m_goalkeeper)
		{
			tempTeamContainingPlayer->m_element.m_num_goalkeeprs--;
		}
		// remove from tree ready teams
		if (!tempTeamContainingPlayer->m_element.check_team_ready())
		{
			this->m_tree_ready_teams->deleteNode(this->m_tree_ready_teams->m_root, playerToRemove->m_element.m_team_id, 0, 0);
			this->m_num_good_teams--;
		}
	}
	// update the team's winning number because this player is removed
	int num = (playerToRemove->m_element.m_goals) - (playerToRemove->m_element.m_cards);
	tempTeamContainingPlayer->m_element.m_winning_num -= num;

	// delete from tree of players by score inside Team
	tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_score->deleteNode(tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_score->m_root, playerToRemove->m_element.m_goals, playerToRemove->m_element.m_cards, playerToRemove->m_element.m_player_id);

	// delete from tree of players by id inside team
	tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_id->deleteNode(tempTeamContainingPlayer->m_element.m_tree_players_in_team_by_id->m_root, playerToRemove->m_element.m_player_id, 0, 0);

	// delete from tree of all players by playerId
	this->m_tree_players_by_id->deleteNode(this->m_tree_players_by_id->m_root, playerId, 0, 0);

	this->m_best_player = this->m_tree_players_by_score->m_highest_key3;
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

	if (!playerToUpdate)
	{
		// player does not exists
		return StatusType::FAILURE;
	}
	int prevCards = playerToUpdate->m_element.m_cards;
	int prevGoals = playerToUpdate->m_element.m_goals;
	int prevGamesPlayed = playerToUpdate->m_element.m_games_played;
	//int prevTeamId = playerToUpdate->m_element.m_team_id;
	bool prevGoalkeeper = playerToUpdate->m_element.m_goalkeeper;
	Node<Team> *prevTeam = playerToUpdate->m_element.m_player_in_team_by_id->m_element.m_my_team;

	// remove the player we want to update from the system using remove player function that has a time complexity of logn
	StatusType res1 = remove_player_except_from_tree_ready_teams(playerId);
	if (res1 != StatusType::SUCCESS)
	{
		return res1;
	}

	// add the player again with updated stats using the add player function that has a time complexity of logn
	StatusType res2 = add_player(playerId, prevTeam, prevGamesPlayed + gamesPlayed, prevGoals + scoredGoals, prevCards + cardsReceived, prevGoalkeeper);
	if (res2 != StatusType::SUCCESS)
	{
		return res2;
	}
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
		}
		else
		{
			team2->m_element.m_points += 3;
		}
		team1->m_element.m_games_played_by_team++;
		team2->m_element.m_games_played_by_team++;
		return StatusType::SUCCESS;
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
	int numGamesPlayedWithTeam = playerToBringGames->m_element.m_player_in_team_by_id->m_element.m_my_team->m_element.m_games_played_by_team;
	return playerToBringGames->m_element.m_games_played + numGamesPlayedWithTeam;
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
	Node<Team> *newTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, newTeamId, 0, 0);
	// check if one or two of the teams dont excise
	if (!team1 || !team2 || ((newTeamId != teamId1 && newTeamId != teamId2) && newTeam))
	{
		//one of the teams does not exist OR a team with the newTeam's ID exists and it isnt one of the given teams
		return StatusType::FAILURE;
	}

	Node<ready_team> *readyTeam1 = this->m_tree_ready_teams->findNode(this->m_tree_ready_teams->m_root, teamId1, 0, 0);
	Node<ready_team> *readyTeam2 = this->m_tree_ready_teams->findNode(this->m_tree_ready_teams->m_root, teamId2, 0, 0);
	bool oneOrBothTeamsWasReady = readyTeam1 || readyTeam2;

	int numPlayersTeam1 = team1->m_element.m_num_players;
	int numPlayersTeam2 = team2->m_element.m_num_players;
	Node<player_in_team> **arrayPlayersID1 = new Node<player_in_team> *[numPlayersTeam1];
	Node<player_in_team> **arrayPlayersScore1 = new Node<player_in_team> *[numPlayersTeam1];
	Node<player_in_team> **arrayPlayersID2 = new Node<player_in_team> *[numPlayersTeam2];
	Node<player_in_team> **arrayPlayersScore2 = new Node<player_in_team> *[numPlayersTeam2];
	Node<player_in_team> **mergeArrayID = new Node<player_in_team> *[(numPlayersTeam1) + (numPlayersTeam2)];
	Node<player_in_team> **mergeArrayScore = new Node<player_in_team> *[(numPlayersTeam1) + (numPlayersTeam2)];

	// create all the arrays
	team1->m_element.m_tree_players_in_team_by_id->inOrderVisitUnite(team1->m_element.m_tree_players_in_team_by_id->m_root, arrayPlayersID1, 0);
	team1->m_element.m_tree_players_in_team_by_score->inOrderVisitUnite(team1->m_element.m_tree_players_in_team_by_score->m_root, arrayPlayersScore1, 0);
	team2->m_element.m_tree_players_in_team_by_id->inOrderVisitUnite(team2->m_element.m_tree_players_in_team_by_id->m_root, arrayPlayersID2, 0);
	team2->m_element.m_tree_players_in_team_by_score->inOrderVisitUnite(team2->m_element.m_tree_players_in_team_by_score->m_root, arrayPlayersScore2, 0);

	// here we might have deleted the contanet of the array !!!!!!
	// dont forget to delete all new
	if (newTeamId != teamId1 && newTeamId != teamId2)
	{	
		// the newTeam is not team 1 or 2
		this->add_team(newTeamId, team1->m_element.m_points + team2->m_element.m_points);
		newTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, newTeamId, 0, 0);

		// if one of the teams was ready before they were united, the new united team will be ready
		if (oneOrBothTeamsWasReady)
		{
			if (readyTeam1)
			{
				this->m_tree_ready_teams->deleteNode(this->m_tree_ready_teams->m_root, teamId1, 0, 0);
				this->m_num_good_teams--;
			}
			if (readyTeam2)
			{
				this->m_tree_ready_teams->deleteNode(this->m_tree_ready_teams->m_root, teamId2, 0, 0);
				this->m_num_good_teams--;
			}
			ready_team newReadyTeam(newTeamId, newTeam);
			this->m_tree_ready_teams->insertNode(this->m_tree_ready_teams->m_root, newTeamId, 0, 0, newReadyTeam);
			this->m_num_good_teams++;
		}
	}
	else if (newTeamId == teamId1)
	{
		// if one of the teams was ready before they were united, the new united team will be ready
		if (readyTeam2)
		{
			if (readyTeam1)
			{
				this->m_tree_ready_teams->deleteNode(this->m_tree_ready_teams->m_root, teamId2, 0, 0);
				this->m_num_good_teams--;
			}
			else
			{
				readyTeam2->m_element.m_team = team1;
				readyTeam2->m_element.m_team_id = teamId1;
			}
		}
	}
	else if (newTeamId == teamId2)
	{
		// if one of the teams was ready before they were united, the new united team will be ready
		if (readyTeam1)
		{
			if (readyTeam2)
			{
				this->m_tree_ready_teams->deleteNode(this->m_tree_ready_teams->m_root, teamId1, 0, 0);
				this->m_num_good_teams--;
			}
			else
			{
				readyTeam1->m_element.m_team = team2;
				readyTeam1->m_element.m_team_id = teamId2;
			}
		}
	}

	for (int i = 0; i < numPlayersTeam1; i++)
	{
		arrayPlayersID1[i]->m_element.m_player->m_element.m_games_played -= team2->m_element.m_games_played_by_team;
		arrayPlayersID1[i]->m_element.m_player->m_element.m_team_id = newTeamId;
        arrayPlayersID1[i]->m_parent = nullptr;
	}
	for (int i = 0; i < numPlayersTeam2; i++)
	{
		arrayPlayersID2[i]->m_element.m_player->m_element.m_games_played -= team1->m_element.m_games_played_by_team;
		arrayPlayersID2[i]->m_element.m_player->m_element.m_team_id = newTeamId;
        arrayPlayersID2[i]->m_parent = nullptr;
	}

	newTeam->m_element.m_games_played_by_team = team1->m_element.m_games_played_by_team + team2->m_element.m_games_played_by_team;
	newTeam->m_element.m_num_goalkeeprs = team1->m_element.m_num_goalkeeprs + team2->m_element.m_num_goalkeeprs;
	newTeam->m_element.m_num_players = team1->m_element.m_num_players + team2->m_element.m_num_players;
	newTeam->m_element.m_winning_num = team1->m_element.m_winning_num + team2->m_element.m_winning_num;
	newTeam->m_element.m_points = team1->m_element.m_points + team2->m_element.m_points;

	newTeam->m_element.m_tree_players_in_team_by_id->merge(arrayPlayersID1, arrayPlayersID2, mergeArrayID, numPlayersTeam1, numPlayersTeam2);
	newTeam->m_element.m_tree_players_in_team_by_score->merge(arrayPlayersScore1, arrayPlayersScore2, mergeArrayScore, numPlayersTeam1, numPlayersTeam2);

	try{
	newTeam->m_element.m_tree_players_in_team_by_id->m_root = newTeam->m_element.m_tree_players_in_team_by_id->arrayToTree(mergeArrayID, 0, (numPlayersTeam1) + (numPlayersTeam2)-1);
	newTeam->m_element.m_tree_players_in_team_by_score->m_root = newTeam->m_element.m_tree_players_in_team_by_score->arrayToTree(mergeArrayScore, 0, (numPlayersTeam1) + (numPlayersTeam2)-1);
	}
	catch(std::bad_alloc &){
		return StatusType::ALLOCATION_ERROR;
	}

	// insert the new united team into the ready teams tree if needed
	if (!oneOrBothTeamsWasReady && newTeam->m_element.check_team_ready())
	{
		ready_team newReadyTeam(newTeamId, newTeam);
		this->m_tree_ready_teams->insertNode(this->m_tree_ready_teams->m_root, newTeamId, 0, 0, newReadyTeam);
		this->m_num_good_teams++;
	}

	// update the heighest key in each tree of players
	newTeam->m_element.m_tree_players_in_team_by_id->m_highest_key1 = mergeArrayID[(numPlayersTeam1) + (numPlayersTeam2)-1]->m_key1;
	newTeam->m_element.m_tree_players_in_team_by_id->m_highest_key2 = mergeArrayID[(numPlayersTeam1) + (numPlayersTeam2)-1]->m_key2;
	newTeam->m_element.m_tree_players_in_team_by_id->m_highest_key3 = mergeArrayID[(numPlayersTeam1) + (numPlayersTeam2)-1]->m_key3;

	newTeam->m_element.m_tree_players_in_team_by_score->m_highest_key1 = mergeArrayScore[(numPlayersTeam1) + (numPlayersTeam2)-1]->m_key1;
	newTeam->m_element.m_tree_players_in_team_by_score->m_highest_key2 = mergeArrayScore[(numPlayersTeam1) + (numPlayersTeam2)-1]->m_key2;
	newTeam->m_element.m_tree_players_in_team_by_score->m_highest_key3 = mergeArrayScore[(numPlayersTeam1) + (numPlayersTeam2)-1]->m_key3;

	delete[] arrayPlayersID1;
	delete[] arrayPlayersID2;
	delete[] arrayPlayersScore1;
	delete[] arrayPlayersScore2;
	delete[] mergeArrayID;
	delete[] mergeArrayScore;

	// remove the old teams after they have been united to a new team under a new name
	if (newTeamId != teamId1 && newTeamId != teamId2)
	{
		team1->m_element.m_num_players = 0;
		team2->m_element.m_num_players = 0;

		this->remove_team(team1->m_element.m_team_id);
		this->remove_team(team2->m_element.m_team_id);
	}
	else if (newTeamId == teamId1)
	{
		team2->m_element.m_num_players = 0;
		this->remove_team(team2->m_element.m_team_id);
	}
	else if (newTeamId == teamId2)
	{
		team1->m_element.m_num_players = 0;
		this->remove_team(team1->m_element.m_team_id);
	}

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
		return this->m_tree_players_by_score->m_highest_key3;
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
		return currentTeam->m_element.m_tree_players_in_team_by_score->m_highest_key3;
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
	return StatusType::FAILURE;
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
    if (this->m_num_players < 2){
        return StatusType::FAILURE;
    }
	Node<Team> *currentTeam = this->m_tree_teams_by_id->findNode(this->m_tree_teams_by_id->m_root, teamId, 0, 0);
    if (!currentTeam){
        return StatusType::FAILURE;
    }
	Node<player_in_team> *playerInTeamById = currentTeam->m_element.m_tree_players_in_team_by_id->findNode(currentTeam->m_element.m_tree_players_in_team_by_id->m_root, playerId, 0, 0);
    if (!playerInTeamById){
        return StatusType::FAILURE;
    }
    //int goals = playerInTeamById->m_element.m_player->m_element.m_goals;
	//int cards = playerInTeamById->m_element.m_player->m_element.m_cards;
	//Node<player_in_team> *playerInTeamByScore = currentTeam->m_element.m_tree_players_in_team_by_score->findNode(currentTeam->m_element.m_tree_players_in_team_by_score->m_root, goals, cards, playerId);

	Node<player_in_scoreboard> *playerInScoreBoard = playerInTeamById->m_element.m_player->m_element.m_player_in_scoreboard;
	Node<player_in_scoreboard> *closest = this->m_tree_players_by_score->closerBetweenTwoOptions(playerInScoreBoard, playerInScoreBoard->m_element.m_next_player_by_score, playerInScoreBoard->m_element.m_prev_player_by_score);

	return closest->m_key3;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	if (minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<ready_team> *currTeam = this->m_tree_ready_teams->m_root;

	Node<ready_team> **goodTeams = new Node<ready_team> *[this->m_num_good_teams];
	//int num_teams = this->m_num_good_teams;
	int numGoodTeams = this->m_tree_ready_teams->inOrderVisitBetweenRange(currTeam, goodTeams, 0, minTeamId, maxTeamId);

	int* winning_nums = new int[numGoodTeams];
	int* teams = new int[numGoodTeams];

	// fill winning nums and teams arrays
	for (int i = 0; i < numGoodTeams; i++)
	{
		winning_nums[i] = goodTeams[i]->m_element.m_team->m_element.m_winning_num;
		teams[i] = goodTeams[i]->m_element.m_team_id;
	}
	// MAKE SURE THIS DOESNT DELETE ALL OF THE VALUES IN THE NODES
	delete[] goodTeams;

	for (int size = numGoodTeams; size > 0; size = std::round(size / 2))
	{
		if (size == 1)
		{
			delete[] winning_nums;
			int id = teams[0];
			delete[] teams;
			return id;
		}
		for (int i = 0; i < size; i += 2)
		{
			if (i + 1 == size)
			{
				winning_nums[i / 2] = winning_nums[i];
				teams[i / 2] = teams[i];
			}
			else
			{
				if (winning_nums[i] < winning_nums[i + 1])
				{
					teams[i / 2] = teams[i + 1];
				}
				else if (winning_nums[i] > winning_nums[i + 1])
				{
					teams[i / 2] = teams[i];
				}
				else
				{
					teams[i / 2] = (teams[i] > teams[i + 1]) ? teams[i] : teams[i + 1];
				}
				winning_nums[i / 2] = winning_nums[i] + winning_nums[i + 1] + 3;
			}
		}
	}
	delete[] winning_nums;
	delete[] teams;

	return -1;
}
