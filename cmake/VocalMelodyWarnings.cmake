function(vocalmelody_enable_warnings target_name)
    if(MSVC)
        target_compile_options(
            ${target_name}
            PRIVATE
                /W4
                /permissive-
                /Zc:__cplusplus
                $<$<BOOL:${VOCALMELODY_WARNINGS_AS_ERRORS}>:/WX>
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(
            ${target_name}
            PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wconversion
                -Wsign-conversion
                $<$<BOOL:${VOCALMELODY_WARNINGS_AS_ERRORS}>:-Werror>
        )
    endif()
endfunction()
