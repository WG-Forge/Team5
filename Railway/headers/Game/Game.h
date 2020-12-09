#pragma once
#include <unordered_map>
#include "Graph.h"
#include "Posts.h"
#include "Player.h"
#include "../ServerConnection/ServerConnection.h"
#include "../ServerConnection/Login.h"

class Game
{
public:
	Game();
	~Game();
	const Graph& GetGraph();
private:
	Graph graph;
	std::unordered_map<int, std::unique_ptr<Post>> idx_to_post;
	Player player;
	ServerConnection connection;
};

