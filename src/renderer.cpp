

#include<glm/vec4.hpp>
#include<glm/vec3.hpp>
#include<GL/glew.h>
#include"renderer.h"


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
}

renderer::Object::~Object() {
	glDeleteBuffers(1, &m_vertexBuffer); // delete the vertex buffer
}

void renderer::Object::GenerateVertexBuffer() {
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, 
		m_verticiesAmount * sizeof(float) * 4, // multiply by 4 since each vertex has 4 components
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

int renderer::Object::GetVertexArrayAmountElements() {return m_verticiesAmount * 4;}



void renderer::ObjectRenderer::SetVertexAttributes() {
	// specify settings for position attributes of verticies
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
}

void renderer::ObjectRenderer::AddObject(Object* object) {
	m_objects.push_back(object);
}

void renderer::ObjectRenderer::RenderAllObjects() {
	for (renderer::Object* obj : m_objects) {
		obj->BindVertexBuffer();
		SetVertexAttributes();
		glDrawArrays(GL_TRIANGLES, 0, obj->GetVertexArrayAmountElements());
	}
}

void renderer::ObjectRenderer::SetShaderProgram(unsigned int shaderProgram) {
	glUseProgram(shaderProgram);
}
