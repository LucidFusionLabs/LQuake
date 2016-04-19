# $Id: CMakeLists.txt 1320 2014-10-26 08:12:52Z justin $

lfl_project(quake)
lfl_add_package(quake SOURCES quake.cpp
                LINK_LIBRARIES ${LFL_APP_LIB} ${LFL_APP_OS} ${LFL_APP_FRAMEWORK} ${LFL_APP_GRAPHICS}
                ${LFL_APP_AUDIO} app_null_camera app_null_matrix app_null_fft
                app_simple_resampler app_simple_loader app_iconv_convert
                app_libpng_png app_null_jpeg app_null_gif app_null_ogg app_null_css app_null_ttf
                app_libarchive_archive app_null_ssl app_null_js app_null_tu)
set(LFL_APP_ASSET_FILES ${LFL_APP_ASSET_FILES} ${LFL_SOURCE_DIR}/core/app/assets/Nobile.*)
lfl_post_build_start(quake quake LQuake)
