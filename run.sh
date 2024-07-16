#!/bin/bash

# Function to configure the project from scratch
project_configure_fresh()
{
    rm -fr build
    mkdir build
    cmake -B build -S .
}

# Function to configure the project
project_configure()
{
    cmake -B build -S .
}

# Function to build the project
project_build()
{
    cmake --build build
}

# Function to run the client
project_run_client()
{
    ./build/main
}

# Function to run the server
project_run_server()
{
    ./build/server
}

# Function to run the debugger for the client
project_run_debugger_for_client()
{
    gdb ./build/main
}

# Function to run the debugger for the server
project_run_debugger_for_server()
{
    gdb ./build/server
}

# Main script execution to handle command-line arguments
case "$1" in
    project-configure-fresh)
        project_configure_fresh
        ;;
    project-configure)
        project_configure
        ;;
    project-build)
        project_build
        ;;
    project-run-client)
        project_run_client
        ;;
    project-run-server)
        project_run_server
        ;;
    project-run-debugger-for-client)
        project_run_debugger_for_client
        ;;
    project-run-debugger-for-server)
        project_run_debugger_for_server
        ;;
    *)
        echo "Usage: $0 {project-configure-fresh|project-configure|project-build|project-run-client|project-run-server|project-run-debugger-for-client|project-run-debugger-for-server}"
        exit 1
        ;;
esac
