project(archiver)

add_executable(archiver
        main.cpp
        archiver_lib/priority_queue.h archiver_lib/priority_queue.cpp
        archiver_lib/binary_trie.h archiver_lib/binary_trie.cpp
        archiver_lib/archiver.h archiver_lib/archiver.cpp
        archiver_lib/unarchiver.h archiver_lib/unarchiver.cpp
        archiver_lib/bit_istream.h archiver_lib/bit_istream.cpp
        archiver_lib/bit_ostream.h archiver_lib/bit_ostream.cpp
)

