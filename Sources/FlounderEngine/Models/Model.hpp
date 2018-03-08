#pragma once

#include <string>
#include <vector>
#include "../Maths/Vector2.hpp"
#include "../Physics/ColliderAabb.hpp"
#include "../Resources/Resources.hpp"
#include "../Renderer/Buffers/VertexBuffer.hpp"
#include "../Renderer/Buffers/IndexBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "Vertex.hpp"
#include "VertexData.hpp"

namespace Flounder
{
	struct F_HIDDEN ModelLoaded
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	/// <summary>
	/// Class that represents a OBJ model.
	/// </summary>
	class F_EXPORT Model :
		public IResource
	{
	private:
		std::string m_filename;

		VertexBuffer *m_vertexBuffer;
		IndexBuffer *m_indexBuffer;

		ColliderAabb *m_aabb;

	public:
		static Model *Resource(const std::string &filename)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<Model *>(resource);
			}

			Model *result = new Model(filename);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		/// <summary>
		/// Creates a new empty model.
		/// </summary>
		Model();

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="filename"> The file name. </param>
		Model(const std::string &filename);

		/// <summary>
		/// Creates a new model.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="indices"> The model indices. </param>
		/// <param name="name"> The model name. </param>
		Model(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices, const std::string &name = "");

		/// <summary>
		/// Creates a new model without indices.
		/// </summary>
		/// <param name="vertices"> The model vertices. </param>
		/// <param name="name"> The model name. </param>
		Model(std::vector<Vertex> &vertices, const std::string &name = "");

		/// <summary>
		/// Deconstructor for the model.
		/// </summary>
		~Model();

		void CmdRender(const VkCommandBuffer &commandBuffer, const unsigned int &instances = 1);

		std::string GetFilename() override { return m_filename; }

		ColliderAabb *GetAabb() const { return m_aabb; }

		VertexBuffer *GetVertexBuffer() const { return m_vertexBuffer; }

		IndexBuffer *GetIndexBuffer() const { return m_indexBuffer; }

	protected:
		void Set(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices, const std::string &name = "");

	private:
		/// <summary>
		/// Loads the model object from a OBJ file.
		/// </summary>
		ModelLoaded LoadFromFile(const std::string &filename);

		VertexData *ProcessDataVertex(const Vector3 &vertex, std::vector<VertexData *> *vertices, std::vector<uint32_t> *indices);

		VertexData *DealWithAlreadyProcessedDataVertex(VertexData *previousVertex, const int &newTextureIndex, const int &newNormalIndex, std::vector<uint32_t> *indices, std::vector<VertexData *> *vertices);

		void CalculateTangents(VertexData *v0, VertexData *v1, VertexData *v2, std::vector<Vector2> *uvs);

		static ColliderAabb CalculateAabb(const std::vector<Vertex> &vertices);
	};
}
