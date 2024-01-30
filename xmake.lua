set_project("ccms")
set_version("0.1.0")
set_languages("c11")

add_rules("mode.debug", "mode.release")

target("ccms")
  set_default(true)
  set_kind("headeronly")
  add_headerfiles("include/(ccms/*.h)")
  add_headerfiles("include/(ccms/arenas/*.h)")
  add_rules("utils.install.cmake_importfiles")
  add_rules("utils.install.pkgconfig_importfiles")

target("examples/main")
  set_enabled(false)
  set_kind("binary")
  add_files("examples/main.c")
  add_includedirs("include")
