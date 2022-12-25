cmake_minimum_required(VERSION 3.7)
project(Flying_FLINGS)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(Flying_FLINGS ${SDL2_INCLUDE_DIRS})
file(GLOB Flying_FLINGS_SOURCE
     "*.h"
     "*.cpp"
)
add_executable(Flying_FLINGS ${Flying_FLINGS_SOURCE})
target_link_libraries(Flying_FLINGS ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)
target_link_libraries(Flying_FLINGS SDL2_ttf::SDL2_ttf)

#TARGET_LINK_LIBRARIES(FlyingFlingsLib -lSDL2)
#TARGET_LINK_LIBRARIES(Flying_Flings PRIVATE SDL2::SDL2)
#TARGET_LINK_LIBRARIES(Flying_Flings -L/Library/Frameworks/SDL2.Framework/)
