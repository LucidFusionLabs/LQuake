# $Id: CMakeLists.txt 1320 2014-10-26 08:12:52Z justin $

lfl_project(quake)
lfl_add_target(quake quake.cpp)
target_compile_definitions(quake PUBLIC ${LFL_APP_DEF})
target_include_directories(quake PUBLIC ${LFL_APP_INCLUDE})
target_link_libraries(quake ${LFL_APP_LIB} ${LFL_APP_OS} ${LFL_APP_FRAMEWORK} ${LFL_APP_GRAPHICS}
                      ${LFL_APP_AUDIO} app_null_camera app_null_matrix app_null_fft
                      app_simple_resampler app_simple_loader app_iconv_convert
                      app_libpng_png app_null_jpeg app_null_gif app_null_css app_null_ttf
                      app_libarchive_archive app_null_ssl)
lfl_post_build_start(quake quake LQuake)
