##
## EPITECH PROJECT, 2026
## Project - Zappy
## File description:
## Main project build file
##

cmake_minimum_required(VERSION 3.15)

set(AI_ENTRY
"\"#!/usr/bin/env" "python3\\n"
"\\nfrom" "ai" "import" "main" "\\n"
"\\ndef" "main()" "->" "None:\\n\\tmain.main()\\n"
"\\nif" "__name__" "==" "'__main__':"
"\\n\\tmain()\\n\""
)

set(AI_SRC_FILES
    main.py)

set(AI_OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/zappy_ai")

add_custom_target(zappy_ai ALL
                  printf ${AI_ENTRY} > "${AI_OUTPUT_PATH}"
                  COMMAND chmod +x "${AI_OUTPUT_PATH}"
                  DEPENDS "${AI_SRC_FILES}")
