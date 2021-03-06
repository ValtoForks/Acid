#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate)
	{
		auto resource = Resources::Get()->Find(ToName(graphicsStage, pipelineCreate));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<PipelineMaterial>(resource);
		}

		auto result = std::make_shared<PipelineMaterial>(graphicsStage, pipelineCreate);
		Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	PipelineMaterial::PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate) :
		Resource(ToName(graphicsStage, pipelineCreate)),
		m_graphicsStage(graphicsStage),
		m_pipelineCreate(pipelineCreate),
		m_renderStage(nullptr),
		m_pipeline(nullptr)
	{
	}

	bool PipelineMaterial::BindPipeline(const CommandBuffer &commandBuffer)
	{
		auto renderStage = Renderer::Get()->GetRenderStage(m_graphicsStage.GetRenderpass());

		if (renderStage == nullptr)
		{
			return false;
		}
		else if (m_renderStage != renderStage)
		{
			m_renderStage = renderStage;
			m_pipeline = std::make_unique<PipelineGraphics>(m_graphicsStage, m_pipelineCreate);
		}

		m_pipeline->BindPipeline(commandBuffer);
		return true;
	}

	std::string PipelineMaterial::ToName(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate)
	{
		std::stringstream result;
		result << "Material_" << graphicsStage.GetRenderpass() << "_" << graphicsStage.GetSubpass() << "_";

		for (auto &stage : pipelineCreate.GetShaderStages())
		{
			result << stage << "_";
		}

		for (auto &element : pipelineCreate.GetDefines())
		{
			result << element.first << "=" << element.second << "_";
		}

		return result.str();
	}
}
