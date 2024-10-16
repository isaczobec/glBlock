#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace renderer {

	class Object {
	private: 
		// transform
		glm::vec4 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		float* m_verticies_positions;
		int m_verticiesAmount;

		unsigned int m_vertexBuffer;
		bool m_vertexBufferExists;

	public:
		Object(float x, float y, float z, float* verticies, int verticiesAmount);
		~Object();
		void GenerateVertexBuffer();
		void DeleteBuffer();
		void BindVertexBuffer();
		int GetVertexArrayAmountElements();

	};



	class ObjectRenderer {
		private:
			std::vector<Object*> m_objects;

		public:
			void SetVertexAttributes();
			void RenderAllObjects();
			void AddObject(Object* object);
			void SetShaderProgram(unsigned int shaderProgram);
	};

}