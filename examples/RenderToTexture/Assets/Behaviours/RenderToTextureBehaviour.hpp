#pragma once

// HG::Core
#include <Behaviour.hpp> // Required for inheritance

namespace HG::Rendering::Base
{
    class RenderTarget;
    class Texture;
    class RenderBehaviour;
}

class RenderToTextureBehaviour : public HG::Core::Behaviour
{
public:

    RenderToTextureBehaviour();

    ~RenderToTextureBehaviour() override;

    void setTarget(HG::Rendering::Base::Texture* texture);

    void setRenderBehaviour(HG::Rendering::Base::RenderBehaviour* behaviour);

protected:
    void onUpdate() override;

private:
    HG::Rendering::Base::RenderTarget* m_renderTarget;
    HG::Rendering::Base::RenderBehaviour* m_renderBehaviour;
};

