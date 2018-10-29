#pragma once

namespace HG::Rendering::Base
{
    class RenderTarget;

    /**
     * @brief Structure, that describes data for rendering
     * override.
     */
    struct RenderOverride
    {
        // Pointer to render target that will be used to render
        // all main rendering behaviours and gizmos. This
        // pointer has to be swapped on usage (to restore 
        // initial render target value)
        HG::Rendering::Base::RenderTarget* mainRenderTarget = nullptr;

    };
}


