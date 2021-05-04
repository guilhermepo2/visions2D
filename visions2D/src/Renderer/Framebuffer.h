#pragma once

namespace visions2D {
	struct FramebufferSpecification {
		unsigned int Width, Height;
		unsigned int Samples = 1;
	};

	class Framebuffer {
		
	public:
		Framebuffer(const FramebufferSpecification& specification);
		~Framebuffer();

		void Bind();
		void Unbind();
		void Invalidate();
		const FramebufferSpecification& GetFramebufferSpecification() const { return m_Specification; }
		const unsigned int GetColorAttachmentID() const { return m_ColorAttachmentID; }

	private:
		unsigned int m_FramebufferID;
		unsigned int m_ColorAttachmentID;
		FramebufferSpecification m_Specification;

	};
}