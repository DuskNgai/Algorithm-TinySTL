set(TestedChapter 2 4 5 6 7 12)

foreach(i ${TestedChapter})
    add_executable(test_chapter_${i} test_chapter_${i}.cpp)
    target_include_directories(test_chapter_${i} PRIVATE ${ALGORITHM_INCLUDES})
    target_link_libraries(test_chapter_${i} PRIVATE ${ALGORITHM_LIBRARIES})
endforeach()
