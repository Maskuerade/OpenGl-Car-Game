# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-src"
  "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-build"
  "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-subbuild/glm-populate-prefix"
  "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-subbuild/glm-populate-prefix/tmp"
  "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-subbuild/glm-populate-prefix/src/glm-populate-stamp"
  "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-subbuild/glm-populate-prefix/src"
  "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-subbuild/glm-populate-prefix/src/glm-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/projects/Graphics Design/project_2/cmake-build-debug/_deps/glm-subbuild/glm-populate-prefix/src/glm-populate-stamp/${subDir}")
endforeach()
