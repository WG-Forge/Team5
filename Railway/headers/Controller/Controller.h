#pragma once
#include "../Game/Game.h"
#include "../ServerConnection/ServerConnection.h"
#include "../ServerConnection/Messages.h"
#include "../Utils/FileLogger.h"
#include "RouteManager.h"
#include <queue>
#include <set>
#include <mutex>

struct SynchronizedGame {
	Game& game;
	std::lock_guard<std::mutex> guard;
};

class Controller
{
public:
	Controller();
	void Init();
	void Disconnect();
	const SynchronizedGame GetGame();
	void MakeTurn();
	int GetTurnNumber() const;
private:
	void UpdateGame();
	void SendMoveRequests(const std::vector<MoveRequest>&);
	void SendUpgradeRequest(std::vector<int>, std::vector<int>);
	void EndTurn();
	void CheckUpgrades();
	bool IsTrainAtHome(int);
	void LogErrorRecieve(const ResposeMessage&, const std::string&);

	int turn_number = 1;
	Game game;
	ServerConnection connection;
	RouteManager route_manager;
	ige::FileLogger logger;
	std::mutex game_mutex;
};

