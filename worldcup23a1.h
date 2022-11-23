// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "wet1util.h"
#include "players.h"
#include "teams.h"

class world_cup_t {
private:
	static int m_best_player;
	static int m_num_players;
	shared_ptr<Player> m_root_players_by_id; //AVL tree containing all of the players in the tournament sorted by ID
	shared_ptr<Player> m_root_players_by_score; //AVL tree containing all of the players in the tournament sorted by score
	shared_ptr<Team> m_root_teams; //AVL tree containing all of the teams in the tournament sorted by ID
    shared_ptr<Team> deleteNode_teams(shared_ptr<Team> node, int teamId);
    shared_ptr<Team> insucv_teams(shared_ptr<Team> node);
    shared_ptr<Team> inprev_teams(shared_ptr<Team> node);
    shared_ptr<Team> rrrotation_teams(shared_ptr<Team> node);
    shared_ptr<Team> llrotation_teams(shared_ptr<Team> node);
    shared_ptr<Team> lrrotation_teams(shared_ptr<Team> node);
    shared_ptr<Team> rlrotation_teams(shared_ptr<Team> node);

    /***********************************************************
    * calc_BF_teams: calculates the balance factor for teams AVL tree - the difference in height between
    * the left and right son-trees of a node in the AVL tree containing the teams
    * @param team the current node
    ***********************************************************/
    int calc_BF_teams(shared_ptr<Team> team);

    /***********************************************************
    * calc_height_teams: calculates the heignt of a node in the AVL tree containing the teams
    ***********************************************************/
    int calc_height_teams(shared_ptr<Team> team);

    /***********************************************************
    * find_team_teams: finds a specific team in the AVL tree containing the teams
    * @param nodeTeam a node in the tree
    * @param teamID the team id number 
    ***********************************************************/
    shared_ptr<Team> find_team_teams(shared_ptr<Team> nodeTeam, int teamID);

    /***********************************************************
    * insert_team_teams: inserts a new team to the AVL tree containing the teams
    * @param nodeTeam a node in the tree
    * @param teamID the new team id number 
    * @param teamID the new team points value
    ***********************************************************/
	shared_ptr<Team> insert_team_teams(shared_ptr<Team> nodeTeam, int teamID, int points);

	/***********************************************************
    * calc_BF_players_ID: calculates the balance factor for players by ID AVL tree - the difference in height between
    * the left and right son-trees of a node in the AVL tree containing the players sorted by ID
    * @param player the current node
    ***********************************************************/
    int calc_BF_players_ID(shared_ptr<Player> player);

    /***********************************************************
    * calc_height_players_ID: calculates the height of a node in the AVL tree containing the players by ID
    ***********************************************************/
    int calc_height_players_ID(shared_ptr<Player> player);

    /***********************************************************
    * find_player_ID: finds a specific player in the AVL tree containing the players by ID
    * @param nodePlayer a node in the tree
    * @param playerID the player id number 
    ***********************************************************/
    shared_ptr<Player> find_player_ID(shared_ptr<Player> nodePlayer, int playerID);

    /***********************************************************
    * insert_player_ID: inserts a new player to the AVL tree containing the players by ID
    * @param nodePlayer a node in the tree
    * @param playerID the new player id number 
	* @param teamId the new player's team
	* @param gamesPlayed how many games this player has played
	* @param goals how many goals this player scored
	* @param cards how many cards this played has
	* @param goalkeeper indicates whether this player can be a goalkeeper or not
    ***********************************************************/
	shared_ptr<Player> insert_player_ID(shared_ptr<Player> nodePlayer, int playerID, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper);

	/***********************************************************
    * calc_BF_players_scores: calculates the balance factor for players by score AVL tree - the difference in height between
    * the left and right son-trees of a node in the AVL tree containing the players sorted by score
    * @param player the current node
    ***********************************************************/
    int calc_BF_players_scores(shared_ptr<Player> player);

    /***********************************************************
    * calc_height_players_score: calculates the height of a node in the AVL tree containing the players by score
    ***********************************************************/
    int calc_height_players_score(shared_ptr<Player> player);

    /***********************************************************
    * find_player_ID: finds a specific player in the AVL tree containing the players by score
    * @param nodePlayer a node in the tree
    * @param playerScore the player score
    ***********************************************************/
    shared_ptr<Player> find_player_score(shared_ptr<Player> nodePlayer, int playerScore);

    /***********************************************************
    * insert_player_ID: inserts a new player to the AVL tree containing the players by score
    * @param nodePlayer a node in the tree
    * @param playerScore the new player score
    ***********************************************************/
	shared_ptr<Player> insert_player_score(shared_ptr<Player> nodePlayer, int playerScore);

    /***********************************************************
    * max: calculates the max between two integers
    * @param a first int
    * @param b second int 
    ***********************************************************/
    static int max(int a, int b);
	
public:
	// <DO-NOT-MODIFY> {
	
	world_cup_t();
	virtual ~world_cup_t();
	
	StatusType add_team(int teamId, int points);
	
	StatusType remove_team(int teamId);
	
	StatusType add_player(int playerId, int teamId, int gamesPlayed,
	                      int goals, int cards, bool goalKeeper);
	
	StatusType remove_player(int playerId);
	
	StatusType update_player_stats(int playerId, int gamesPlayed,
	                                int scoredGoals, int cardsReceived);
	
	StatusType play_match(int teamId1, int teamId2);
	
	output_t<int> get_num_played_games(int playerId);
	
	output_t<int> get_team_points(int teamId);
	
	StatusType unite_teams(int teamId1, int teamId2, int newTeamId);
	
	output_t<int> get_top_scorer(int teamId);
	
	output_t<int> get_all_players_count(int teamId);
	
	StatusType get_all_players(int teamId, int *const output);
	
	output_t<int> get_closest_player(int playerId, int teamId);
	
	output_t<int> knockout_winner(int minTeamId, int maxTeamId);
	
	// } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
