#include "Railway.h"
#include <algorithm>
#include "SFML/Graphics.hpp"
#include <fstream>
#include <iostream>
#include "../JsonUtils/ParseUtils.h"
#include "Drawer.h"
#include "Camera.h"
#include "../ServerConnection/ServerConnection.h"
#include "../ServerConnection/Login.h"
#include "../ServerConnection/Messages.h"
#include "../JsonUtils/ParseUtils.h"

Railway::Railway(int winWidth, int winHeight) 
	: windowWidth{std::max(winWidth, 0)}, windowHeight{std::max(winHeight, 0)} {}
	
void Railway::start() {
	Game game;
	const Graph& graph = game.GetGraph();

	//PlaceGraph(graph, 500.f, 50.f, 0.1f, 500.f);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Railway");
	
	sf::View camera(sf::FloatRect(0.f, 0.f, static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
	
	Drawer drawer;
	
	sf::Font label_font;
	label_font.loadFromFile("fonts\\jai.ttf");

	FocusOnGraph(camera, graph);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		
		window.setView(camera);
		//drawer.visualUpdate(graph);
		drawer.drawGraph(window, graph);
		drawer.drawLabels(window, graph, label_font);
		
		window.setView(window.getDefaultView());
		window.display();
	}
}