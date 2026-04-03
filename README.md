# FTP: Proof Of Concept

This repo is a 'FTP **Proof Of Concept**' project.   
The aim of this project is to resimulate basic FTP over a network   

*this is a component for a bigger project*

## DESCRIPTION
This repo is a recreation of an FTP server not following all the RFC Standards but follows general convention 
user can upload file, download files and review logs

The architecture uses a dual-channel design:

Control server (commands and control flow)
Data server (file transfer operations)

## Requirements

- Compiler: clang++
- Standard: C++17
- Recommended OS: Linux / Unix-like environment

## Building the Project

Build all components:

### BUILDING PROJECT

#### RUN SERVERS
> make run-server

#### RUN CLIENT
> make run-client

## CLEANING THE PROJECT
***remove compiled binaries***
> make clean



### CURRENT FEATURES
- Upload files using up command
- Download files using down command
- Concurrent uploads and downloads
- Dual-channel architecture
- Multi-threaded connection handling
- Logging system for debugging and monitoring

### NOTE
This project is intended for educational purposes and focuses on networking fundamentals, multi-threading, file I/O operations, and protocol design.

## License

This project is for educational use only.