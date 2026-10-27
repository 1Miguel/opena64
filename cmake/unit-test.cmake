macro(unity_enable)

  include(FetchContent)

  message(STATUS "fetch unity from git")
  FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.7.0
  )

  message(STATUS "make unity available")

  FetchContent_MakeAvailable(unity)

endmacro()
