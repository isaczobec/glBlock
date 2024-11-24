#include <InputManager.h>
#include<GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <cmath>

InputManager* InputManager::m_Instance = nullptr;

InputManager::InputManager(GLFWwindow* window) {
	
	// initialize members
	m_Instance = this; // set the pointer to the current instance
	m_currentMovementInput = glm::vec2(0.0, 0.0); // set the movement input vector to 0

	m_mousePositionCurrent = glm::vec2(0.0, 0.0);
	m_mousePositionPrevious = glm::vec2(0.0, 0.0);
	m_mousePositionDelta = glm::vec2(0.0, 0.0);
	m_mouseDeltaMin = 2; // set the minimum delta for the mouse to move

	// enable raw mouse input and disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	// set the key callback
	glfwSetKeyCallback(window, UpdateCurrentMovementInput);
	glfwSetCursorPosCallback(window, UpdateMousePosition);
	
}

void InputManager::UpdateCurrentMovementInput(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		// jättesnyggt verkligen
		switch (key)
		{
		default:
			break;
		case GLFW_KEY_A:
			m_Instance->m_currentMovementInput.x = m_Instance->m_currentMovementInput.x - 1.0;
			break;
		case GLFW_KEY_D:
			m_Instance->m_currentMovementInput.x = m_Instance->m_currentMovementInput.x + 1.0;
			break;
		case GLFW_KEY_W:
			m_Instance->m_currentMovementInput.y = m_Instance->m_currentMovementInput.y - 1.0;
			break;
		case GLFW_KEY_S:
			m_Instance->m_currentMovementInput.y = m_Instance->m_currentMovementInput.y + 1.0;
			break;
		}
	}
}

void InputManager::UpdateMousePosition(GLFWwindow* window, double xpos, double ypos) { 
	m_Instance->m_mousePositionPrevious = m_Instance->m_mousePositionCurrent;
	m_Instance->m_mousePositionCurrent = glm::vec2(xpos, ypos);
	m_Instance->m_mousePositionDelta = m_Instance->m_mousePositionCurrent - m_Instance->m_mousePositionPrevious;


	if (std::abs(m_Instance->m_mousePositionDelta.x) < m_Instance->m_mouseDeltaMin) {
		m_Instance->m_mousePositionDelta.x = 0.0;
	}
	if (std::abs(m_Instance->m_mousePositionDelta.y) < m_Instance->m_mouseDeltaMin) {
		m_Instance->m_mousePositionDelta.y = 0.0;
	}
}

glm::vec2 InputManager::GetMousePositionDelta() {
	return m_mousePositionDelta;
}

void InputManager::ResetMovementVector() {
	// reset input vector
	m_currentMovementInput.x = 0.0;
	m_currentMovementInput.y = 0.0;
}

glm::vec2 InputManager::GetMovementInput() {
	return m_currentMovementInput;
}

void InputManager::Refresh() {
	// reset input vector
	ResetMovementVector();
}

