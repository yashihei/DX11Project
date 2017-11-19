// -----------------------------------------------
// File : InputManager.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <memory>
#include "DirectXTK/Keyboard.h"
#include "DirectXTK/GamePad.h"

class InputManager
{
public:
	InputManager() : m_padIndex(0)
	{
		m_keyboard = std::make_unique<DirectX::Keyboard>();
		m_gamePad = std::make_unique<DirectX::GamePad>();
	}

	bool isPressedButton1() const { return m_keyboard->GetState().Z || m_gamePad->GetState(m_padIndex).IsYPressed(); }
	bool isPressedButton2() const { return m_keyboard->GetState().X || m_gamePad->GetState(m_padIndex).IsBPressed(); }
	bool isPressedButton3() const { return m_keyboard->GetState().C || m_gamePad->GetState(m_padIndex).IsAPressed(); }
	bool isPressedButton4() const { return m_keyboard->GetState().V || m_gamePad->GetState(m_padIndex).IsXPressed(); }
	bool isPressedUp() const { return m_keyboard->GetState().Up || m_gamePad->GetState(m_padIndex).dpad.up; }
	bool isPressedDown() const { return m_keyboard->GetState().Down || m_gamePad->GetState(m_padIndex).dpad.down; }
	bool isPressedLeft() const { return m_keyboard->GetState().Left || m_gamePad->GetState(m_padIndex).dpad.left; }
	bool isPressedRight() const { return m_keyboard->GetState().Right || m_gamePad->GetState(m_padIndex).dpad.right; }

	bool isClicledButton1() const { return m_keyTracker.pressed.Z || m_padTracker.y == ButtonState::PRESSED; }
	bool isClicledButton2() const { return m_keyTracker.pressed.X || m_padTracker.b == ButtonState::PRESSED; }
	bool isClicledButton3() const { return m_keyTracker.pressed.C || m_padTracker.a == ButtonState::PRESSED; }
	bool isClicledButton4() const { return m_keyTracker.pressed.V || m_padTracker.x == ButtonState::PRESSED; }
	bool isClicledUp() const { return m_keyTracker.pressed.Up || m_padTracker.dpadUp == ButtonState::PRESSED; }
	bool isClicledDown() const { return m_keyTracker.pressed.Down || m_padTracker.dpadDown == ButtonState::PRESSED; }
	bool isClicledLeft() const { return m_keyTracker.pressed.Left || m_padTracker.dpadLeft == ButtonState::PRESSED; }
	bool isClicledRight() const { return m_keyTracker.pressed.Right || m_padTracker.dpadRight == ButtonState::PRESSED; }

	bool isRelesedButton1() const { return m_keyTracker.released.Z || m_padTracker.y == ButtonState::RELEASED; }
	bool isRelesedButton2() const { return m_keyTracker.released.X || m_padTracker.b == ButtonState::RELEASED; }
	bool isRelesedButton3() const { return m_keyTracker.released.C || m_padTracker.a == ButtonState::RELEASED; }
	bool isRelesedButton4() const { return m_keyTracker.released.V || m_padTracker.x == ButtonState::RELEASED; }
	bool isRelesedUp() const { return m_keyTracker.released.Up || m_padTracker.dpadUp == ButtonState::RELEASED; }
	bool isRelesedDown() const { return m_keyTracker.released.Down || m_padTracker.dpadDown == ButtonState::RELEASED; }
	bool isRelesedLeft() const { return m_keyTracker.released.Left || m_padTracker.dpadLeft == ButtonState::RELEASED; }
	bool isRelesedRight() const { return m_keyTracker.released.Right || m_padTracker.dpadRight == ButtonState::RELEASED; }

	bool isConnectedPad() const { return m_gamePad->GetState(m_padIndex).IsConnected(); }
	void changePadIndex(const int index) { m_padIndex = index; }
	float getLeftThumbX() const { return m_gamePad->GetState(m_padIndex).thumbSticks.leftX; }
	float getLeftThumbY() const { return m_gamePad->GetState(m_padIndex).thumbSticks.leftY; }
	float getRightThumbX() const { return m_gamePad->GetState(m_padIndex).thumbSticks.rightX; }
	float getRightThumbY() const { return m_gamePad->GetState(m_padIndex).thumbSticks.rightY; }

	void update()
	{
		m_keyTracker.Update(m_keyboard->GetState());
		if (m_gamePad->GetState(m_padIndex).IsConnected())
			m_padTracker.Update(m_gamePad->GetState(m_padIndex));
	}

private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::GamePad> m_gamePad;
	DirectX::Keyboard::KeyboardStateTracker m_keyTracker;
	DirectX::GamePad::ButtonStateTracker m_padTracker;
	using ButtonState = DirectX::GamePad::ButtonStateTracker::ButtonState;
	int m_padIndex;
};
