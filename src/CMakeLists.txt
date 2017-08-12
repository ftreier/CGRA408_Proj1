#
# CGRA Project
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"cgra_math.hpp"
	"stb_image_write.h"
	"geometry.hpp"
	"shape.hpp"
	"sphere.hpp"
	"ray.hpp"
	"camera.hpp"
	"film.hpp"
	"helper.hpp"
	"renderer.hpp"
	"light.hpp"
	"distantLight.hpp"
	"material.hpp"
)

# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"geometry.cpp"
	"shape.cpp"
	"sphere.cpp"
	"ray.cpp"
	"camera.cpp"
	"film.cpp"
	"helper.cpp"
	"renderer.cpp"
	"light.cpp"
	"distantLight.cpp"
	"material.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${CGRA_PROJECT} ${headers} ${sources})
#target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})