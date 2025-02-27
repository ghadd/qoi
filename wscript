def options(opt):
    opt.load('compiler_cxx')


def configure(conf):
    conf.load('compiler_cxx')

    # Check if the compiler is MSVC or GCC/Clang
    if conf.env.CC_NAME == 'msvc':
        # MSVC uses /std:c++20 instead of -std=c++20
        conf.env.CXXFLAGS = ['/std:c++20', '/DNOMINMAX']
    else:
        # GCC and Clang use -std=c++20
        conf.env.CXXFLAGS = ['-std=c++20']


def build(bld):
    bld.program(
        target='qoiconv',
        source='qoiconv.c',
        includes=['.', 'stb']
    )

    bld.program(
        target='cli',
        source='cli.cpp',
        includes=['.', 'stb']
    )