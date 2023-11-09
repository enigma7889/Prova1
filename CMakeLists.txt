cmake_minimum_required(VERSION 3.16)
project(prova1 VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

# add_executable(main main.cpp particletype.cpp particle.cpp resonancetype.cpp)

# se il testing e' abilitato...
# per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  # aggiungi l'eseguibile particletype.t
  add_executable(particletype.t test/particletype.test.cpp particletype.cpp resonancetype.cpp)
  # aggiungi l'eseguibile particletype.t alla lista dei test
  add_test(NAME particletype.t COMMAND particletype.t)
  
  # aggiungi l'eseguibile particle.t
  add_executable(particle.t test/particle.test.cpp particle.cpp resonancetype.cpp particletype.cpp)
  # aggiungi l'eseguibile particle.t alla lista dei test
  add_test(NAME particle.t COMMAND particle.t)

  # aggiungi l'eseguibile resonancetype.t
  add_executable(resonancetype.t test/resonancetype.test.cpp resonancetype.cpp particletype.cpp)
  # aggiungi l'eseguibile resonancetype.t alla lista dei test
  add_test(NAME resonancetype.t COMMAND resonancetype.t)

endif()

