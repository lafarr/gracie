cmake_minimum_required(VERSION 3.10)

project(engine)

file(GLOB_RECURSE GRACIE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
list(FILTER GRACIE_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY clang-tidy REQUIRED)

set(CMAKE_CXX_CLANG_TIDY
    ${CLANG_TIDY}
	--use-color
    --extra-arg=-Wno-unknown-warning-option
    --warnings-as-errors=*
    --header-filter=${CMAKE_SOURCE_DIR}/.*
)

add_library(settings INTERFACE)

target_compile_options(settings INTERFACE
	-Wall
	-Wextra
	-Wpedantic
	-Wno-unused-parameter
	-Wunused
	-Wunused-variable
	-Wunused-result
	-Winline                       
	-Wdisabled-optimization        
	-Wmissing-noreturn
	-Wpessimizing-move             
	-Wredundant-move               
	-Wmove                         
	-Wshift-overflow
	-Wshift-sign-overflow
	-Winteger-overflow
	-Wdivision-by-zero
	-Wnull-dereference
	-Wstrict-overflow=5            
	-Warray-bounds
	-Wsequence-point               
	-Wfree-nonheap-object
	-Waddress                      
	-Wmismatched-new-delete
	-Wcast-align                   
	-Wcast-qual                    
	-Wold-style-cast               
	-Wsign-compare
	-Wsign-conversion
	-Wdouble-promotion             
	-Wfloat-conversion
	-Wnarrowing
	-Wconversion                   
	-Wint-to-pointer-cast
	-Wpointer-arith
	-Wshadow                       
	-Wshadow-all                   
	-Wswitch-enum                  
	-Wswitch-default               
	-Wimplicit-fallthrough         
	-Wdeprecated                   
	-Wdeprecated-copy              
	-Wdeprecated-copy-dtor
	-Wzero-as-null-pointer-constant
	-Wnon-virtual-dtor             
	-Woverloaded-virtual           
	-Wextra-semi
	-Wunreachable-code
	-Wunreachable-code-return
	-Wmissing-field-initializers
	-Wredundant-decls
	-Wctor-dtor-privacy            
	-Wnoexcept-type
	-Wregister                     
	-Wcomma                        
	-Wformat=2                     
	-Wformat-security
	-Wformat-overflow
	-Wformat-truncation
	-fstrict-aliasing                 
	-Wpadded                          
	-Wnoexcept-type                   
	-Wmissing-noreturn                

	$<$<CONFIG:Debug>:
		-fsanitize=undefined           
		-fsanitize=address             
		-fsanitize=leak                
		-Og
	>

	$<$<CONFIG:Release>:
        -O3
		-ggdb3
        -march=native
        -fno-exceptions
        -fno-rtti
    >
)

target_link_options(settings INTERFACE
	$<$<CONFIG:Debug>:
		-fsanitize=address
		-fsanitize=undefined
		-fsanitize=leak
	>
)

add_library(gracie_core ${GRACIE_SOURCES})
target_link_libraries(gracie_core PUBLIC settings)
target_include_directories(gracie_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(engine PRIVATE gracie_core)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_executable(gracie_tests tests/move_gen_tests.cpp)
target_link_libraries(gracie_tests PRIVATE gracie_core settings)
target_include_directories(gracie_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME gracie_tests COMMAND gracie_tests)
