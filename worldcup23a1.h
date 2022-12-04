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

#include <cmath>
#include "wet1util.h"
#include "AVLtree.h"
#include "player.h"
#include "ready_team.h"


class world_cup_t {
private:
	int m_best_player;
	int m_num_players;
	int m_num_good_teams;
	AVLtree<player>* m_tree_players_by_id; //AVL tree containing all of the players in the tournament sorted by ID
	AVLtree<player_in_scoreboard>* m_tree_players_by_score; //AVL tree containing all of the players in the tournament sorted by score
    AVLtree<Team>* m_tree_teams_by_id; //AVL tree containing all of the teams in the tournament sorted by id
    AVLtree<ready_team>* m_tree_ready_teams; //AVL tree containing all of the teams in the tournament that are ready to play (have 11 players, one of them is a goalie) sorted by id

	StatusType add_player(int playerId, Node<Team>* team, int gamesPlayed,
	                      int goals, int cards, bool goalKeeper);
	StatusType remove_player_except_from_tree_ready_teams(int playerId);


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
