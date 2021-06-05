FetchContent_Declare(
	catch
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG v2.13.4
)
FetchContent_MakeAvailable(catch)

add_executable(tests main.cpp gamemodes_test.cpp)
target_compile_features(tests PRIVATE cxx_std_20)

target_link_libraries(tests PRIVATE doppelkopf Catch2::Catch2)

add_test(NAME libdoppelkopftest COMMAND tests)