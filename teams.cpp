#include "teams.h"

int Team::max(int a, int b)
{
    return (a > b) ? a : b;
}

int Team::calc_height(shared_ptr<Team> node)
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

int Team::calc_BF(shared_ptr<Team> team)
{
    if (!team)
    {
        return 0;
    }
    if (team->m_left_team && team->m_left_team)
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

shared_ptr<Team> Team::find_team(shared_ptr<Team> nodeTeam, int teamID)
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
        find_team(nodeTeam->m_left_team, teamID);
    }
    if (teamID > nodeTeam->m_team_id)
    {
        find_team(nodeTeam->m_right_team, teamID);
    }
}

shared_ptr<Team> Team::insert_team(shared_ptr<Team> nodeTeam, int teamID, int points)
{
    if (nodeTeam == nullptr)
    {
        shared_ptr<Team> root = make_shared<Team>(teamID, points);
        return root;
    }
    if (teamID < nodeTeam->m_team_id)
    {
        nodeTeam->m_left_team = insert_team(nodeTeam->m_left_team, teamID, points);
    }
    if (teamID > nodeTeam->m_team_id)
    {
        nodeTeam->m_right_team = insert_team(nodeTeam->m_right_team, teamID, points);
    }
    nodeTeam->m_height_teams = calc_height(nodeTeam);

    int balance_factor = calc_BF(nodeTeam);
    if (balance_factor == 2 && calc_BF(nodeTeam->m_left_team) == 1)
    {
        // LL ROTATION
    }
    else if (balance_factor == -2 && calc_BF(nodeTeam->m_right_team) == -1)
    {
        // RR ROTATION
    }
    else if (balance_factor == -2 && calc_BF(nodeTeam->m_right_team) == 1)
    {
        // RL ROTATION
    }
    else if (balance_factor == 2 && calc_BF(nodeTeam->m_left_team) == -1)
    {
        // LR ROTATION
    }
}
