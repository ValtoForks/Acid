#pragma once

#include <vector>
#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Particles.hpp"

namespace acid
{
	class ACID_EXPORT RendererParticles :
		public RenderPipeline
	{
	private:
		PipelineGraphics m_pipeline;
		UniformHandler m_uniformScene;
	public:
		explicit RendererParticles(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Camera &camera) override;
	};
}
