set_project("ccms")
set_version("0.1.0")
set_languages("c11")

add_rules("mode.debug", "mode.release")

--[[
This script sets up a target named "ccms" which is a header-only library.

The target is set as the default target and includes all header files in the
"include/ccms/**" directories.

Two rules are added for the target: one for auto-updating compile commands and
another for installing CMake and pkg-config import files.
]]
add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })
target("ccms")
  set_default(true)
  set_kind("headeronly")
  add_headerfiles("include/(ccms/*.h)")
  add_headerfiles("include/(ccms/arena/*.h)")
  add_includedirs("include", { public = true })
  add_rules("utils.install.cmake_importfiles")
  add_rules("utils.install.pkgconfig_importfiles")

target("examples/main")
  set_enabled(true)
  set_kind("binary")
  add_files("examples/main.c")
  add_deps("ccms")

--[[
This script is used to create a separate xmake target for each C file in the
test directory that matches the pattern "test__*.c".

For each target, it sets the kind to "binary", excludes it from the default
build, and adds the corresponding C file to the build files.

It also adds a dependency on the "ccms" target, adds a "default" test with plain
output, and sets a policy that the test should return zero on failure.
]]
for _, file in ipairs(os.files("test/test__*.c")) do
  -- Extract the base name of the file
  local name = path.basename(file)

  -- Create a new target with the base name of the file
  target(name)
    set_kind("binary")
    set_default(false)
    add_files("test/" .. name .. ".c")
    add_deps("ccms")
    add_tests("default", { plain = true })
    set_policy("test.return_zero_on_failure", true)
end