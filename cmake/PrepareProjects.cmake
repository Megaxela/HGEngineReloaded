function(prepare_imgui_project PATH)

    add_library(imgui STATIC
        ${PATH}/imgui.cpp
        ${PATH}/imgui_draw.cpp
        ${PATH}/imgui_widgets.cpp
    )

    target_include_directories(imgui
        PUBLIC ${PATH}
    )

endfunction()