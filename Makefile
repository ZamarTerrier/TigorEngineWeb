
all: libTigorEngineWeb.a clean

%.o: src/%.c
    emcc -sUSE_SDL=2 -sFULL_ES2=1 -sWASM=0 -sEXPORTED_FUNCTIONS="['_processData', '_malloc']" -Iincludes -Ilibs  -sEXPORT_ALL -c $< 

libTigorEngineWeb.a: %.o
    emar -cr $@  $<

clean:
    $(RM) *.o

#call emcc -sUSE_SDL=2 -sFULL_ES2=1 -sWASM=0 -sEXPORTED_FUNCTIONS="['_processData', '_malloc']" -Iincludes -Ilibs  -sEXPORT_ALL -c src/e_audioObject.c 
#src/e_camera.c src/e_math.c src/e_memory.c src/e_texture.c src/e_transform.c src/engine.c src/gameObject.c src/gameObject2D.c src/gameObject3D.c src/graphicsObject.c 
#src/input_manager.c src/intersections2D.c src/shape_object.c src/sprite_object.c src/primitive_object.c src/web_init.c src/web_window.c src/window_manager.c src/GUIManager.c src/GUIWidgets.c src/TigorGUI.c src/e_tools.c
#call emar -cr build/libTigorEngineWeb.a e_audioObject.o e_camera.o e_math.o e_memory.o e_texture.o e_transform.o engine.o gameObject.o gameObject2D.o graphicsObject.o input_manager.o intersections2D.o shape_object.o sprite_object.o web_init.o web_window.o window_manager.o GUIManager.o GUIWidgets.o TigorGUI.o e_tools.o
#call del *.o
