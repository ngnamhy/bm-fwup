cmake_minimum_required(VERSION 4.0)
project(maximal)

set(CMAKE_CXX_STANDARD 20)

cmake_policy(SET CMP0167 OLD)



# Ask for the components you need
find_package(Boost REQUIRED COMPONENTS 
        log 
        log_setup
        thread 
        filesystem 
        date_time 
        regex 
        chrono 
        atomic)

find_package(yaml-cpp REQUIRED)



set(ALGO
        algo/algo.h
		algo/bmfwup.cpp
		algo/bmfwup.h
		algo/wunminer.cpp
		algo/wunminer.h
)

set(DS
        ds/transaction.cpp
        ds/transaction.h
        ds/database.cpp
        ds/database.h
		ds/umnnode.cpp
		ds/umnnode.h
		ds/umntree.cpp
		ds/umntree.h
		ds/umncode.cpp
		ds/umncode.h
        ds/nlist.cpp
        ds/nlist.h
        ds/itemset.cpp
        ds/itemset.h
	ds/item.h 
	ds/item.h 
)

set(UTILS
        utils/input.cpp
        utils/input.h
        utils/logger.cpp
        utils/logger.h
        utils/config.cpp
        utils/config.h
        utils/stat.cpp
        utils/stat.h
        utils/common.cpp
        utils/common.h
        utils/variable.cpp
        utils/variable.h
)

add_executable(maximal
        main.cpp
        ${ALGO}
        ${DS}
        ${UTILS}
)

include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(maximal PRIVATE
        Boost::log
        Boost::log_setup
        Boost::thread
        Boost::filesystem
        Boost::date_time
        Boost::regex
        Boost::chrono
        Boost::atomic
        Boost::boost
        yaml-cpp

)
