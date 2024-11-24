#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
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

		unsigned int m_shaderProgram;

	public:
		Object(float x, float y, float z, float* verticies, int verticiesAmount);
		~Object();
		void GenerateVertexBuffer();
		void DeleteBuffer();
		void BindVertexBuffer();
		int GetVertexArrayAmountElements();
		void SetShaderProgram(unsigned int shaderProgram);
		unsigned int GetShaderProgram();

	};




	class Camera {

	private:

		float m_fov;
		float m_aspectRatio; // x/y
		float m_nearPlane;
		float m_farPlane;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::mat4x4 m_clipSpaceMatrix;
		void CalculateViewMatrix();

		glm::vec3 GetLookDirection();
		
	public:
		Camera(float fov, float aspectRatio, float nearPlane, float farPlane);
		void UpdateTransform(glm::vec3 deltaPosition, glm::vec3 deltaRotation);
		glm::mat4x4* GetClipSpaceMatrix();

	};

	class ObjectRenderer {
		private:
			std::vector<Object*> m_objects;
			Camera* m_camera;

		public:
			void SetVertexAttributes();
			void RenderAllObjects();
			void AddObject(Object* object);
			void SetShaderProgram(unsigned int shaderProgram);
			ObjectRenderer(Camera* camera);
	};


	void PrintMatrix4(glm::mat4x4 matrix);

}