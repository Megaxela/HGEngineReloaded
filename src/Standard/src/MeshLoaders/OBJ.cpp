#include "MeshLoaders/OBJ.hpp"
#include <Mesh.hpp>
#include <sstream>
#include <algorithm>


std::map<
    std::string_view,
    std::function<
        void(
            ::HG::Utils::MeshPtr&,       // mesh ptr
            std::string_view::const_iterator&, // iterator
            std::string_view&                  // line
        )
    >
> HG::Standard::MeshLoaders::OBJ::m_tokenProcessors = {
    {"v", // Vertex
     [](::HG::Utils::MeshPtr& mesh,
        std::string_view::const_iterator& iterator,
        std::string_view& line)
     {
         ::HG::Utils::Vertex vertex;

         // Searching for 3 coordinates. Skipping fourth for now
         auto space = std::find(
             iterator,
             line.end(),
             ' '
         );

//         std::from_chars(iterator, line.end());
     }
    }
};

HG::Standard::MeshLoaders::OBJ::OBJ()
{

}

::HG::Utils::MeshPtr HG::Standard::MeshLoaders::OBJ::load(::HG::Core::ResourceAccessor::DataPtr data)
{
    auto castedData =
        std::string_view(
            reinterpret_cast<const char*>(data->data()),
            data->size()
        );

    auto mesh = std::make_shared<::HG::Utils::Mesh>();

    // Parsing line by line
    auto iterator = castedData.begin();


    while (iterator <= castedData.end())
    {
        // If it's comment, just skip the line
        if (*iterator == '#')
        {
            // Find newline
            iterator = std::find(
                iterator,
                castedData.end(),
                '\n'
            );

            // Find something that is not a space
            iterator = std::find_if(
                iterator,
                castedData.end(),
                [](char v)
                {
                    return v != ' ';
                }
            );

            continue;
        }

        // Getting line
        std::string_view line(
            // From our current location
            iterator,
            // Until newline
            static_cast<std::string_view::size_type>(
                // Searching length from iterator to end of line
                std::distance(
                    // From begin
                    iterator,
                    // Until end of line or end of string
                    std::find(
                        iterator,
                        castedData.end(),
                        '\n'
                    )
                )
            )
        );

        // If it's empty newline
        if (line.empty())
        {
            iterator = line.end() + 1;
        }

        // Getting first token
        std::string_view token(
            iterator,
            static_cast<std::string_view::size_type>(
                std::distance(
                    iterator,
                    std::find(
                        iterator,
                        line.end(),
                        ' '
                    )
                )
            )
        );


    }

    return mesh;
}
