// HG::Rendering::Base
#include <HG/Rendering/Base/BlitData.hpp>

HG::Rendering::Base::BlitData::BlitData() : m_points()
{
}

void HG::Rendering::Base::BlitData::blitRectangular(HG::Rendering::Base::Texture* texture,
                                                    glm::ivec2 tl,
                                                    glm::ivec2 br,
                                                    glm::ivec2 pos,
                                                    glm::ivec2 size)
{
    if (size.x == 0 || size.y == 0)
    {
        size = br - tl;
    }

    blitMesh(texture,
             {{pos, tl},
              {pos + glm::ivec2(size.x, 0), {br.x, tl.y}},
              {pos + glm::ivec2(0, size.y), {tl.x, br.y}},
              {pos + size, br}},
             {0, 1, 2, 2, 1, 3});
}

void HG::Rendering::Base::BlitData::blitMesh(HG::Rendering::Base::Texture* texture,
                                             const std::vector<HG::Rendering::Base::BlitData::PointData>& points,
                                             const std::vector<uint32_t>& indices)
{
    auto& pointData = m_points[texture];

    auto indicesOffset = static_cast<uint32_t>(pointData.vertices.size());

    for (const auto& index : indices)
    {
        pointData.indices.push_back(index + indicesOffset);
    }

    for (const auto& point : points)
    {
        pointData.vertices.emplace_back(point);
    }
}

const HG::Rendering::Base::BlitData::BlitContainer& HG::Rendering::Base::BlitData::blitContainer() const
{
    return m_points;
}
