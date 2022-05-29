add_executable(game
        Game.cpp
        InputContext.cpp
        Timer.cpp
        Texture.cpp
        Tileset.cpp
        controls.cpp
        UI/dialog.cpp
        UI/menu.cpp

        MapData.cpp
        Map.cpp

        Objects/Animation.cpp
        Objects/Movement.cpp
        drawable/Character.cpp
        drawable/CharacterPlayer.cpp

        input/dbgmenu.cpp
        input/editor.cpp
        input/player.cpp
        interfaces/Drawable.h input/text_select.cpp input/text_select.h utility/Utility.cpp utility/Utility.h Renderer.cpp)


target_link_libraries(game ${SDL2_LIBRARIES})
