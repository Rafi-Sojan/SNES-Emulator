#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <array>
#include <filesystem>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Joypad {
public:

	// these are my preferrable controller settings i use for playing most of the games in snes, preferrably for Super Metroid, you can change the controls as you wish through the window interface after i have done developing it 
	Joypad() = default;
	void movementinput() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			d_up();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			d_down();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			d_right();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			d_left();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
			d_X();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
			d_Y();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
			d_A();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
			d_B();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			d_RS();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			d_LS();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
			d_Start();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
			d_Select();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I))
			increasebrightness();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O))
			decreasebrightness();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
			forceblank();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1))
			savestate();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2))
			loadstate();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			speedemulation();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSlash))
			stopemulation();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3))
			restartemulation();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F4))
			changefilters();
	}

private:
	void d_up();
	void d_down();
	void d_left();
	void d_right();
	void increasebrightness();
	void decreasebrightness();
	void d_A();
	void d_B();
	void d_Y();
	void d_X();
	void d_LS();
	void d_RS();
	void d_Start();
	void d_Select();
	void forceblank();
	void stopemulation();
	void speedemulation();
	void changefilters();
	void restartemulation();
	void savestate();
	void loadstate();
};