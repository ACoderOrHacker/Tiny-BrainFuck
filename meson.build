project('bf', ['c', 'nasm'],
    default_options : ['c_std=c99'])


add_project_arguments(
    '-ffreestanding',
    '-nostdinc',
    '-fno-stack-protector',
    '-fno-builtin',
    language : 'c'
)

add_project_link_arguments(
    '-nostdlib',
    '-static',
    '-Wl,-e,_start',
    '-Wl,--no-dynamic-linker',
    language : 'c'
)

executable('bf',
    sources : [
        'src/brainfuck.c',
        'src/linux.asm'
    ],
    nasm_args : ['-f', 'elf64'],
    include_directories: include_directories('src')
)
