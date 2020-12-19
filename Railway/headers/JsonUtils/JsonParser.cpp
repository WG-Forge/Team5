#include "JsonParser.h"

Graph JsonParser::ParseGraph(std::string& input) {
	using namespace rapidjson;
	Document doc;
	doc.Parse(input.c_str());
	const auto& name = doc["name"].GetString();
	const auto& ind = doc["idx"].GetInt();
	const auto& points = doc["points"].GetArray();
	const auto& lines = doc["lines"].GetArray();

	Graph graph;

	for (const auto& item : points) {
		graph.AddVertex({
			item["idx"].GetInt(),
			item["post_idx"].IsInt() ? item["post_idx"].GetInt() : -1,
			{0.0f, 0.0f},
			});
	}

	for (const auto& item : lines) {
		const auto& v = item["points"].GetArray();
		int idx = item["idx"].GetInt();
		int length = item["length"].GetInt();
		int v1 = v[0].GetInt();
		int v2 = v[1].GetInt();
		graph.AddEdge(
			{ idx, length },
			v1,
			v2
		);
		graph.AddEdge(
			{ idx, length },
			v2,
			v1
		);
	}
	return graph;
}

Player JsonParser::ParsePlayer(std::string& input) {
	using namespace rapidjson;
	using namespace std;
	Document doc;
	doc.Parse(input.c_str());

	const auto& home_item = doc["home"].GetObject();
	Home home = { home_item["idx"].GetInt(), home_item["post_idx"].GetInt() };

	string idx = doc["idx"].GetString();
	bool in_game = doc["in_game"].GetBool();
	string name = doc["name"].GetString();
	int rating = doc["rating"].GetInt();

	const auto& trains_item = doc["trains"].GetArray();
	vector<Train> trains = ParseTrains(trains_item);

	const auto& town_item = doc["town"].GetObject();
	auto town_ptr = ParseTown(town_item);
	return Player{ home, move(idx), in_game, move(name), rating, move(*town_ptr), move(trains) };
}

std::unordered_map<int, std::shared_ptr<Post>> JsonParser::ParsePosts(std::string& input) {
	using namespace rapidjson;
	using namespace std;
	Document doc;
	doc.Parse(input.c_str());

	unordered_map<int, shared_ptr<Post>> result;

	const auto& posts_array = doc["posts"].GetArray();
	for (const auto& item : posts_array) {
		PostType type = static_cast<PostType>(item["type"].GetInt());
		shared_ptr<Post> post_ptr;
		switch (type) {
		case PostType::TOWN:
			post_ptr = ParseTown(item);
			break;
		case PostType::STORAGE:
			post_ptr = ParseStorage(item);
			break;
		case PostType::MARKET:
			post_ptr = ParseMarket(item);
			break;
		}
		result[post_ptr->point_idx] = move(post_ptr);
	}
	return result;
}

std::vector<std::pair<int, Point>> JsonParser::ParseCoordinates(std::string& input) {
	using namespace rapidjson;
	Document doc;
	doc.Parse(input.c_str());

	std::vector<std::pair<int, Point>> res;
	const auto& coord_item = doc["coordinates"].GetArray();
	for (const auto& item : coord_item) {
		res.push_back({ item["idx"].GetInt(),
			{static_cast<float>(item["x"].GetInt()), static_cast<float>(item["y"].GetInt())} });
	}
	//to do: parse idx and size
	return res;
}

std::vector<Train> JsonParser::ParseTrains(const rapidjson::GenericArray<false, rapidjson::Value>& array) {
	std::vector<Train> trains;
	for (const auto& item : array) {
		Train train;
		train.cooldown = item["cooldown"].GetInt();
		//train.fuel = item["fuel"].GetInt();
		//train.fuel_capacity = item["fuel_capacity"].GetInt();
		train.goods = item["goods"].GetInt();
		train.goods_capacity = item["goods_capacity"].GetInt();
		const auto& goods_type_item = item["goods_type"];
		if (goods_type_item.IsString()) {
			train.goods_type = goods_type_item.GetString() == "product" ?
				GoodsType::PRODUCT : GoodsType::ARMOR;
		}
		else {
			train.goods_type = GoodsType::NONE;
		}
		train.idx = item["idx"].GetInt();
		train.level = item["level"].GetInt();
		train.line_idx = item["line_idx"].GetInt();
		train.next_level_price = item["next_level_price"].GetInt();
		train.player_idx = item["player_idx"].GetString();
		train.position = item["position"].GetInt();
		train.speed = item["speed"].GetInt();
	}
	return trains;
}

std::shared_ptr<Town> JsonParser::ParseTown(const rapidjson::Value& town_item) {
	auto town = std::make_unique<Town>();
	town->armor = town_item["armor"].GetInt();
	town->armor_capacity = town_item["armor_capacity"].GetInt();
	town->idx = town_item["idx"].GetInt();
	town->level = town_item["level"].GetInt();
	town->name = town_item["name"].GetString();
	town->next_level_price = town_item["next_level_price"].GetInt();
	if (town_item["player_idx"].IsString()) {
		town->player_idx = town_item["player_idx"].GetString();
	}
	else {
		town->player_idx = "null";
	}
	town->point_idx = town_item["point_idx"].GetInt();
	town->population = town_item["population"].GetInt();
	town->population_capacity = town_item["population_capacity"].GetInt();
	town->product = town_item["product"].GetInt();
	town->product_capacity = town_item["product_capacity"].GetInt();
	town->train_cooldown = town_item["train_cooldown"].GetInt();
	town->type = PostType::TOWN;
	return town;
}

std::shared_ptr<Market> JsonParser::ParseMarket(const rapidjson::Value& market_item) {
	auto market = std::make_unique<Market>();
	market->idx = market_item["idx"].GetInt();
	market->name = market_item["name"].GetString();
	market->point_idx = market_item["point_idx"].GetInt();
	market->product = market_item["product"].GetInt();
	market->product_capacity = market_item["product_capacity"].GetInt();
	market->type = PostType::MARKET;
	return market;
}

std::shared_ptr<Storage> JsonParser::ParseStorage(const rapidjson::Value& storage_item) {
	auto storage = std::make_unique<Storage>();
	storage->armor = storage_item["armor"].GetInt();
	storage->armor_capacity = storage_item["armor_capacity"].GetInt();
	storage->idx = storage_item["idx"].GetInt();
	storage->name = storage_item["name"].GetString();
	storage->point_idx = storage_item["point_idx"].GetInt();
	storage->type = PostType::STORAGE;
	return storage;
}
