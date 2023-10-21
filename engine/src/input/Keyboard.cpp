#include "PCH.h"
#include "Keyboard.h"

namespace Keyboard {

std::set<Keyboard::Key> keysDown = {};

std::string toString(Key k) {
	std::string s;
	char c[2];
	if (k >= Key::NUM0 && k <= Key::NUM9) {
		c[0] = (static_cast<char>(k) - static_cast<char>(Keyboard::Key::NUM0) + '0');
	} else if (k >= Key::A && k <= Key::Z) {
		c[0] = (static_cast<char>(k) - static_cast<char>(Keyboard::Key::A) + 'A');
	}
	c[1] = '\0';
	return s.append(c);
}

void keyDown(Keyboard::Key k) {
	keysDown.insert(k);
}

void keyUp(Keyboard::Key k) {
	keysDown.erase(k);
}

}
