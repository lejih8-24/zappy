##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Main project build file
##

cmake_minimum_required(VERSION 3.15)

set(AI_ENTRY
"\"#!/usr/bin/env" "python3\\n"
"\\nimport" "sys\\n"
"\\nsys.path.append(\\\"${CMAKE_SOURCE_DIR}/ai\\\")\\n"
"\\nimport" "main" "as" "ai\\n"
"\\ndef" "main()" "->" "None:\\n\\tai.main()\\n"
"\\nif" "__name__" "==" "'__main__':"
"\\n\\tmain()\\n\""
)

set(AI_OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/zappy_ai")

add_custom_target(zappy_ai ALL
                  printf ${AI_ENTRY} > "${AI_OUTPUT_PATH}"
                  COMMAND chmod +x "${AI_OUTPUT_PATH}")
