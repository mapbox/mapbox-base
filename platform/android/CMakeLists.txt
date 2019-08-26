if(PLATFORM STREQUAL android)
    set(ANDROID_PLATFORM_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "Android platform Dir" FORCE)

    # Add platform specific source files to a given target
    function(mapbox_base_android_sources target)
        target_sources(${target} PRIVATE
            ${ANDROID_PLATFORM_DIR}/src/async_task.cpp
            ${ANDROID_PLATFORM_DIR}/src/jni.cpp
            ${ANDROID_PLATFORM_DIR}/src/run_loop.cpp
            ${ANDROID_PLATFORM_DIR}/src/run_loop_impl.hpp
            ${ANDROID_PLATFORM_DIR}/src/thread.cpp
            ${ANDROID_PLATFORM_DIR}/src/timer.cpp
        )
    endfunction()

    # Specify location of platform specific headers to
    # a given target.
    function(mapbox_base_android_includes target)
        target_include_directories(${target} PRIVATE
            ${ANDROID_PLATFORM_DIR}/src
        )
    endfunction()
endif()
