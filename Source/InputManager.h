// -----------------------------------------------
// File : InputManager.h
// Copyright (c) 2017 yashihei
// Licensed under the MIT license
// -----------------------------------------------

#pragma once

#include <memory>
#include <DirectXTK/Keyboard.h>
#include <DirectXTK/GamePad.h>
#include <DirectXTK/Mouse.h>
#include "Types.h"

namespace hks {

class InputManager {
public:
	InputManager() : m_padIndex(0)
	{
		m_keyboard = std::make_unique<DirectX::Keyboard>();
		m_mouse = std::make_unique<DirectX::Mouse>();
		m_gamePad = std::make_unique<DirectX::GamePad>();
	}

	bool isPressedButton() const { return isPressedButton1() | isPressedButton2() | isPressedButton3() | isPressedButton4(); }
	bool isPressedButton1() const { return m_keyboard->GetState().Z || m_gamePad->GetState(m_padIndex).IsYPressed(); }
	bool isPressedButton2() const { return m_keyboard->GetState().X || m_gamePad->GetState(m_padIndex).IsBPressed(); }
	bool isPressedButton3() const { return m_keyboard->GetState().C || m_gamePad->GetState(m_padIndex).IsAPressed(); }
	bool isPressedButton4() const { return m_keyboard->GetState().V || m_gamePad->GetState(m_padIndex).IsXPressed(); }
	bool isPressedUp() const { return m_keyboard->GetState().Up || m_keyboard->GetState().W || m_gamePad->GetState(m_padIndex).dpad.up; }
	bool isPressedDown() const { return m_keyboard->GetState().Down || m_keyboard->GetState().S || m_gamePad->GetState(m_padIndex).dpad.down; }
	bool isPressedLeft() const { return m_keyboard->GetState().Left || m_keyboard->GetState().A || m_gamePad->GetState(m_padIndex).dpad.left; }
	bool isPressedRight() const { return m_keyboard->GetState().Right || m_keyboard->GetState().D || m_gamePad->GetState(m_padIndex).dpad.right; }

	bool isClickedButton() const { return isClickedButton1() | isClickedButton2() | isClickedButton3() | isClickedButton4(); }
	bool isClickedButton1() const { return m_keyTracker.pressed.Z || m_padTracker.y == PadButtonState::PRESSED; }
	bool isClickedButton2() const { return m_keyTracker.pressed.X || m_padTracker.b == PadButtonState::PRESSED; }
	bool isClickedButton3() const { return m_keyTracker.pressed.C || m_padTracker.a == PadButtonState::PRESSED; }
	bool isClickedButton4() const { return m_keyTracker.pressed.V || m_padTracker.x == PadButtonState::PRESSED; }
	bool isClickedDebugButton() const { return m_keyTracker.pressed.F1; }
	bool isClickedUp() const { return m_keyTracker.pressed.Up || m_padTracker.dpadUp == PadButtonState::PRESSED; }
	bool isClickedDown() const { return m_keyTracker.pressed.Down || m_padTracker.dpadDown == PadButtonState::PRESSED; }
	bool isClickedLeft() const { return m_keyTracker.pressed.Left || m_padTracker.dpadLeft == PadButtonState::PRESSED; }
	bool isClickedRight() const { return m_keyTracker.pressed.Right || m_padTracker.dpadRight == PadButtonState::PRESSED; }

	bool isRelesedButton1() const { return m_keyTracker.released.Z || m_padTracker.y == PadButtonState::RELEASED; }
	bool isRelesedButton2() const { return m_keyTracker.released.X || m_padTracker.b == PadButtonState::RELEASED; }
	bool isRelesedButton3() const { return m_keyTracker.released.C || m_padTracker.a == PadButtonState::RELEASED; }
	bool isRelesedButton4() const { return m_keyTracker.released.V || m_padTracker.x == PadButtonState::RELEASED; }
	bool isRelesedUp() const { return m_keyTracker.released.Up || m_padTracker.dpadUp == PadButtonState::RELEASED; }
	bool isRelesedDown() const { return m_keyTracker.released.Down || m_padTracker.dpadDown == PadButtonState::RELEASED; }
	bool isRelesedLeft() const { return m_keyTracker.released.Left || m_padTracker.dpadLeft == PadButtonState::RELEASED; }
	bool isRelesedRight() const { return m_keyTracker.released.Right || m_padTracker.dpadRight == PadButtonState::RELEASED; }

	bool isConnectedPad() const { return m_gamePad->GetState(m_padIndex).IsConnected(); }
	void changePadIndex(const int index) { m_padIndex = index; }
	float getLeftThumbX() const { return m_gamePad->GetState(m_padIndex, DirectX::GamePad::DEAD_ZONE_CIRCULAR).thumbSticks.leftX; }
	float getLeftThumbY() const { return m_gamePad->GetState(m_padIndex, DirectX::GamePad::DEAD_ZONE_CIRCULAR).thumbSticks.leftY; }
	float getRightThumbX() const { return m_gamePad->GetState(m_padIndex, DirectX::GamePad::DEAD_ZONE_CIRCULAR).thumbSticks.rightX; }
	float getRightThumbY() const { return m_gamePad->GetState(m_padIndex, DirectX::GamePad::DEAD_ZONE_CIRCULAR).thumbSticks.rightY; }

	Vector2 getMousePos() const { return { static_cast<float>(m_mouse->GetState().x), static_cast<float>(m_mouse->GetState().y) }; }

	void update()
	{
		m_keyTracker.Update(m_keyboard->GetState());

		if (m_mouse->IsConnected())
			m_mouseTracker.Update(m_mouse->GetState());

		if (m_gamePad->GetState(m_padIndex).IsConnected())
			m_padTracker.Update(m_gamePad->GetState(m_padIndex));
	}
private:
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::GamePad> m_gamePad;
	DirectX::Keyboard::KeyboardStateTracker m_keyTracker;
	DirectX::Mouse::ButtonStateTracker m_mouseTracker;
	DirectX::GamePad::ButtonStateTracker m_padTracker;
	using PadButtonState = DirectX::GamePad::ButtonStateTracker::ButtonState;
	using MouseButtonState = DirectX::Mouse::ButtonStateTracker::ButtonState;
	int32 m_padIndex;
};

} // namespace hks
