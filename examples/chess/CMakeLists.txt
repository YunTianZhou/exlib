add_executable(chess 
	"main.cpp"
	"board.cpp"
	"game_model.cpp"
	"game_renderer.cpp"
	"game_controller.cpp"
)

target_link_libraries(chess ${EXLIB_LINK_TARGET})
