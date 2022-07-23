add_executable(game
        Main.cpp
        InputManager.cpp
        Timer.cpp
        Texture.cpp
        Tileset.cpp
        controls.cpp
        UI/dialog.cpp
        UI/menu.cpp

        World/MapData.cpp
        World/Map.cpp

        Objects/Animation.cpp
        Objects/Movement.cpp
        drawable/Character.cpp
        drawable/CharacterPlayer.cpp

        input/dbgmenu.cpp
        input/editor.cpp
        input/player.cpp
        interfaces/Drawable.h
        input/text_select.cpp input/text_select.h
        Game.cpp Game.h
        Objects/FrameConfig.cpp Objects/FrameConfig.h
        RenderManager.cpp RenderManager.h
        TextureManager.cpp TextureManager.h
        World/Definitions.h
)


target_link_libraries(game ${SDL2_LIBRARIES})
