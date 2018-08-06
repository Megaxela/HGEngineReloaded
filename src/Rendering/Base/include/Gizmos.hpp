#pragma once

#include <glm/vec3.hpp>
#include <utility>
#include <Transform.hpp>
#include <Color.hpp>


namespace RENDERING_BASE_MODULE_NS
{
    /**
     * @brief Class, that describe object/interface for working with
     * debug draw.
     */
    class Gizmos
    {
    public:

        static UTILS_MODULE_NS::Color defaultColor;

        using LineSegment = std::pair<glm::vec3, glm::vec3>;

        using Line = std::vector<glm::vec3>;

        // todo: Add commentary
        struct SphereData
        {
            SphereData() :
                transformation(),
                dividings(15),
                radius(1),
                color()
            {}

            SphereData(const CORE_MODULE_NS::Transform& transformation,
                       int dividings,
                       float radius,
                       const UTILS_MODULE_NS::Color& color) :
                transformation(transformation),
                dividings(dividings),
                radius(radius),
                color(color)
            {}

            CORE_MODULE_NS::Transform transformation;
            int dividings;
            float radius;
            UTILS_MODULE_NS::Color color;
        };

        // todo: Add commentary
        struct HexahedronData
        {
            HexahedronData() :
                transformation(),
                sideSize(1),
                color()
            {}

            HexahedronData(const CORE_MODULE_NS::Transform& transformation,
                           float sideSize,
                           const UTILS_MODULE_NS::Color& color) :
                transformation(transformation),
                sideSize(sideSize),
                color(color)
            {}

            CORE_MODULE_NS::Transform transformation;
            float sideSize;
            UTILS_MODULE_NS::Color color;
        };

        // todo: Add commentary
        struct LineData
        {
            LineData() :
                begin(),
                beginColor(),
                end(),
                endColor()
            {}

            LineData(const glm::vec3& begin,
                     const UTILS_MODULE_NS::Color& beginColor,
                     const glm::vec3& end,
                     const UTILS_MODULE_NS::Color& endColor) :
                begin(begin),
                beginColor(beginColor),
                end(end),
                endColor(endColor)
            {}

            glm::vec3 begin;
            UTILS_MODULE_NS::Color beginColor;

            glm::vec3 end;
            UTILS_MODULE_NS::Color endColor;
        };

        /**
         * @brief Method for drawing line.
         * @param from From point.
         * @param to To point.
         * @todo Move color arg to const &
         */
        void line(const glm::vec3& from,
                  const glm::vec3& to,
                  UTILS_MODULE_NS::Color color=defaultColor);

        /**
         * @brief Method for drawing line with
         * gradient color.
         * @param from From point.
         * @param fromColor From point color.
         * @param to To point.
         * @param toColor To point color.
         */
        void line(const glm::vec3& from,
                  const UTILS_MODULE_NS::Color& fromColor,
                  const glm::vec3& to,
                  const UTILS_MODULE_NS::Color& toColor);

        /**
         * @brief Method for drawing circle on x/y without any rotation.
         * @param position Position.
         * @param radius Radius.
         * @param color Color.
         */
        void circle(const glm::vec3& position,
                    float radius,
                    const UTILS_MODULE_NS::Color& color=defaultColor);

        /**
         * @brief Method for drawing crcle on x/y without any rotation.
         * @param position Position.
         * @param size Size.
         * @param color Color.
         */
        void circle(const glm::vec3& position,
                    glm::vec2 size,
                    const UTILS_MODULE_NS::Color& color=defaultColor);

        /**
         * @brief Method for drawing circle.
         * @param transform Transformation (scale will affect size)
         * @param size Circle size.
         * @param parts Number of parts in circle.
         * @param color Circle color.
         */
        void circle(const CORE_MODULE_NS::Transform& transform,
                    glm::vec2 size,
                    uint32_t parts,
                    const UTILS_MODULE_NS::Color& color);

        /**
         * @brief Method for drawing line of multiple segments.
         * @param line Line data.
         */
        void multiLine(const Line& lineData,
                       UTILS_MODULE_NS::Color color=defaultColor);

        /**
         * @brief Method for drawing line.
         * @param lineSegment Line segment.
         */
        void line(const LineSegment& lineSegment,
                  UTILS_MODULE_NS::Color color=defaultColor);

        /**
         * @brief Method for drawing sphere.
         * @param transform Sphere transformation.
         * @param radius Sphere radius. (Transformation scale will affect sphere "size")
         * @param color Sphere color.
         */
        void sphere(const CORE_MODULE_NS::Transform &transform,
                    float radius,
                    UTILS_MODULE_NS::Color color=defaultColor);

        /**
         * @brief Method for drawing sphere.
         * @param transform Sphere transformation.
         * @param radius Sphere radius. (Transformation scale will affect sphere "size")
         * @param dividings Sphere dividing quality.
         * @param color Sphere color.
         */
        void sphere(const CORE_MODULE_NS::Transform &transform,
                    float radius,
                    int dividings,
                    UTILS_MODULE_NS::Color color = defaultColor);

        /**
         * @brief Method for drawing hexahedron.
         * @param transform Hexahedron transformation.
         * @param sideSize Size of hexahedron side.
         * @param color Hexahedron color.
         */
        void hexahedron(const CORE_MODULE_NS::Transform &transform,
                        float sideSize,
                        UTILS_MODULE_NS::Color color=defaultColor);

        /**
         * @brief Template method for getting shapes, that was prepared
         * for rendering.
         *
         * Visitor must implement following method:
         * ```
         * void line(const LineData& line);
         * void sphere(const SphereData& sphere);
         * void hexahedron(const HexahedronData& hexahedron);
         * ```
         *
         * @tparam ShapeVisitor Shape visitor type. (Static polymorphism)
         * @param visitor Actual shape visitor object.
         */
        template<typename ShapeVisitor>
        void visitShapes(ShapeVisitor& visitor) const
        {
            for (auto&& line : m_linesBuffer)
            {
                visitor.line(line);
            }

            for (auto&& sphere : m_sphereBuffer)
            {
                visitor.sphere(sphere);
            }

            for (auto&& hexahedron : m_hexahedronBuffer)
            {
                visitor.hexahedron(hexahedron);
            }
        }

        /**
         * @brief Method for clearing gizmos.
         */
        void clear();

    private:

        std::vector<LineData> m_linesBuffer;
        std::vector<SphereData> m_sphereBuffer;
        std::vector<HexahedronData> m_hexahedronBuffer;

    };
}


