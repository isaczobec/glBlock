

#include<glm/vec4.hpp>
#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>
#include<glm/ext/matrix_transform.hpp>
#include<glm/ext/matrix_clip_space.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GL/glew.h>
#include"renderer.h"
#include <iostream>


// object class definitions

renderer::Object::Object(float x, float y, float z, float* verticies, int verticiesAmount) {

	// set transform
	m_position = glm::vec4(x, y, z, 1);
	m_rotation = glm::vec3(0, 0, 0);
	m_scale = glm::vec3(0, 0, 0);

	m_verticies_positions = verticies;
	m_verticiesAmount = verticiesAmount;
	m_vertexBufferExists = false;
	m_vertexBuffer = 0;

	m_shaderProgram = 0; // leave uninitialized
}

renderer::Object::~Object() {
	glDeleteBuffers(1, &m_vertexBuffer); // delete the vertex buffer
}

void renderer::Object::GenerateVertexBuffer() {
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, 
		m_verticiesAmount * sizeof(float) * 3, // multiply by 4 since each vertex has 4 components
		m_verticies_positions, 
		GL_STATIC_DRAW);
}

void renderer::Object::DeleteBuffer()
{
	// delete buffer if it already exists
	if (m_vertexBufferExists) {
		glDeleteBuffers(1, &m_vertexBuffer);
	}
}

void renderer::Object::BindVertexBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
}

int renderer::Object::GetVertexArrayAmountElements() { return m_verticiesAmount * 3; }
void renderer::Object::SetShaderProgram(unsigned int shaderProgram) { m_shaderProgram = shaderProgram; }
unsigned int renderer::Object::GetShaderProgram() { return m_shaderProgram; }




void renderer::ObjectRenderer::SetVertexAttributes() {
	// specify settings for position attributes of verticies
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
}

void renderer::ObjectRenderer::AddObject(Object* object) {
	m_objects.push_back(object);
}

void renderer::ObjectRenderer::RenderAllObjects() {
	for (renderer::Object* obj : m_objects) {
		obj->BindVertexBuffer();
		SetVertexAttributes();

		unsigned int objShaderProgram = obj->GetShaderProgram();
		SetShaderProgram(objShaderProgram);


		// set uniforms
		int location = glGetUniformLocation(objShaderProgram, "u_projectionMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(*(m_camera->GetClipSpaceMatrix())));

		glDrawArrays(GL_TRIANGLES, 0, obj->GetVertexArrayAmountElements());
	}
}

void renderer::ObjectRenderer::SetShaderProgram(unsigned int shaderProgram) {
	glUseProgram(shaderProgram);
}


renderer::ObjectRenderer::ObjectRenderer(Camera* camera) {
	m_camera = camera;
}


// ------ camera -------

void renderer::Camera::CalculateViewMatrix() {
	// calculate view matrix
	glm::mat4x4 viewMatrix = glm::translate(glm::mat4(1.0f), -m_position); // move
	viewMatrix = glm::rotate(viewMatrix, m_rotation.y, glm::vec3(-1, 0, 0)); // rotate y
	viewMatrix = glm::rotate(viewMatrix, m_rotation.x, glm::vec3(0, 1, 0)); // rotate x
	glm::mat4x4 projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane); // transform to clip space, z is depth

	m_clipSpaceMatrix = projectionMatrix * viewMatrix; // set view matrix

}

void renderer::Camera::UpdateTransform(glm::vec3 deltaPosition, glm::vec3 deltaRotation) {
	m_position += deltaPosition.z * GetLookDirection();
	m_rotation += deltaRotation;
	m_rotation.y = glm::clamp(m_rotation.y, -glm::pi<float>() / 2, glm::pi<float>() / 2); // clamp x rotation

	CalculateViewMatrix(); // recalculate view matrix
}

renderer::Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane) {
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	m_position = glm::vec3(0, 0, 0);
	m_rotation = glm::vec3(0, 0, 0);

	CalculateViewMatrix();
}

glm::mat4x4* renderer::Camera::GetClipSpaceMatrix() {
	return &m_clipSpaceMatrix;
}



void renderer::PrintMatrix4(glm::mat4x4 matrix) {
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			std::cout << matrix[j][i] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

glm::vec3 renderer::Camera::GetLookDirection() {
	float pi = glm::pi<float>();
	float x = cos(m_rotation.x + pi/2.0) * cos(m_rotation.y);
	float y = sin(m_rotation.y);
	float z = sin(m_rotation.x + pi/2.0) * cos(m_rotation.y);

	return glm::vec3(x,y,z);
}