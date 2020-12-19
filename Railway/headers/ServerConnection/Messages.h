#pragma once
#include <string>
#include "../JsonUtils/JsonWriter.h"
#include "Login.h"

enum class Action {
	LOGIN = 1,
	LOGOUT = 2,
	MOVE = 3,
	UPGRADE = 4,
	TURN = 5,
	PLAYER = 6,
	GAMES = 7,
	MAP = 10
};

enum class Result {
	OKEY = 0,
	BAD_COMMAND = 1,
	RESOURCE_NOT_FOUND = 2,
	ACCESS_DENIED = 3,
	INAPPROPRIATE_GAME_STATE = 4,
	TIMEOUT = 5,
	INTERNAL_SERVER_ERROR = 500
};

struct ActionMessage {
	Action actionCode;
	std::string data;
	ActionMessage(const Login& login) : actionCode(Action::LOGIN), data(JsonWriter::WriteLogin(login)) {}
	ActionMessage(const Action& actionCode, const std::string& data) : actionCode(actionCode), data(data) {}
};

struct ResposeMessage {
	Result result;
	std::string data;
	ResposeMessage() : result(Result::ACCESS_DENIED), data("") {}
	ResposeMessage(const Result& result, const std::string& data) : result(result), data(data) {}
};