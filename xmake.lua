add_requires("raylib")

target("squarepp")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("raylib")
