#pragma once

#include<GLFW/glfw3.h>
#include <glm/vec2.hpp>


class InputManager {

private:
	glm::vec2 m_currentMovementInput;

	glm::vec2 m_mousePositionCurrent;
	glm::vec2 m_mousePositionPrevious;
	glm::vec2 m_mousePositionDelta;
	float m_mouseDeltaMin;


	static InputManager* m_Instance;
	static void UpdateCurrentMovementInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void UpdateMousePosition(GLFWwindow* window, double xpos, double ypos);
	void ResetMovementVector();

public:

	InputManager(GLFWwindow* window); // set callbacks in constructor
	glm::vec2 GetMovementInput();
	glm::vec2 GetMousePositionDelta();

	void Refresh();

	
};