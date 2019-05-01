add_subdirectory(cryptoCpp)

file(GLOB SOURCES
    "*.h"
    "*.cpp"
)
add_executable(SerenityTestTask ${SOURCES})

target_link_libraries(SerenityTestTask
  cryptopp-static
)

