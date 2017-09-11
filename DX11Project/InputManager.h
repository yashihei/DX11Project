#pragma once

#include <memory>
#include <DirectXMath.h>
#include "DirectXTK/Keyboard.h"
#include "DirectXTK/GamePad.h"

class InputManager {
public:
	InputManager() {
		m_keyboard = std::make_unique<DirectX::Keyboard>();
		m_gamePad = std::make_unique<DirectX::GamePad>();
	}

	bool isPressedButton1() { return m_keyboard->GetState().Z     || m_gamePad->GetState(0).IsYPressed(); }
	bool isPressedButton2() { return m_keyboard->GetState().X     || m_gamePad->GetState(0).IsBPressed(); }
	bool isPressedButton3() { return m_keyboard->GetState().C     || m_gamePad->GetState(0).IsAPressed(); }
	bool isPressedButton4() { return m_keyboard->GetState().V     || m_gamePad->GetState(0).IsXPressed(); }
	bool isPressedUp()      { return m_keyboard->GetState().Up    || m_gamePad->GetState(0).dpad.up; }
	bool isPressedDown()    { return m_keyboard->GetState().Down  || m_gamePad->GetState(0).dpad.down; }
	bool isPressedLeft()    { return m_keyboard->GetState().Left  || m_gamePad->GetState(0).dpad.left; }
	bool isPressedRight()   { return m_keyboard->GetState().Right || m_gamePad->GetState(0).dpad.right; }

	bool isClicledButton1() { return m_keyTracker.pressed.Z     || m_padTracker.y == ButtonState::PRESSED; }
	bool isClicledButton2() { return m_keyTracker.pressed.X     || m_padTracker.b == ButtonState::PRESSED; }
	bool isClicledButton3() { return m_keyTracker.pressed.C     || m_padTracker.a == ButtonState::PRESSED; }
	bool isClicledButton4() { return m_keyTracker.pressed.V     || m_padTracker.x == ButtonState::PRESSED; }
	bool isClicledUp()      { return m_keyTracker.pressed.Up    || m_padTracker.dpadUp == ButtonState::PRESSED; }
	bool isClicledDown()    { return m_keyTracker.pressed.Down  || m_padTracker.dpadDown == ButtonState::PRESSED; }
	bool isClicledLeft()    { return m_keyTracker.pressed.Left  || m_padTracker.dpadLeft == ButtonState::PRESSED; }
	bool isClicledRight()   { return m_keyTracker.pressed.Right || m_padTracker.dpadRight == ButtonState::PRESSED; }

	bool isRelesedButton1() { return m_keyTracker.released.Z     || m_padTracker.y == ButtonState::RELEASED; }
	bool isRelesedButton2() { return m_keyTracker.released.X     || m_padTracker.b == ButtonState::RELEASED; }
	bool isRelesedButton3() { return m_keyTracker.released.C     || m_padTracker.a == ButtonState::RELEASED; }
	bool isRelesedButton4() { return m_keyTracker.released.V     || m_padTracker.x == ButtonState::RELEASED; }
	bool isRelesedUp()      { return m_keyTracker.released.Up    || m_padTracker.dpadUp == ButtonState::RELEASED; }
	bool isRelesedDown()    { return m_keyTracker.released.Down  || m_padTracker.dpadDown == ButtonState::RELEASED; }
	bool isRelesedLeft()    { return m_keyTracker.released.Left  || m_padTracker.dpadLeft == ButtonState::RELEASED; }
	bool isRelesedRight()   { return m_keyTracker.released.Right || m_padTracker.dpadRight == ButtonState::RELEASED; }

	void update() {
		m_keyTracker.Update(m_keyboard->GetState());
		if (m_gamePad->GetState(0).IsConnected()) m_padTracker.Update(m_gamePad->GetState(0));
	}
private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::GamePad> m_gamePad;
	DirectX::Keyboard::KeyboardStateTracker m_keyTracker;
	DirectX::GamePad::ButtonStateTracker m_padTracker;
	using ButtonState = DirectX::GamePad::ButtonStateTracker::ButtonState;
};
