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

## HOW TO USE

### Starting the system
1. Compile files using the current MAKEFILE
2. Open two separate terminals 
> Terminal 1 : make run-server
> Terminal 2 : make run-client 
3. In the same terminal as **make run-client**

## 2. Available Commands

Once the client is running, you can use the following primary commands:

| Command | Description |
|--------|-------------|
| ls | List Files: Requests a list of all files currently stored in the server's `files/` directory. |
| up | Upload: Initiates a file transfer from your local machine to the server. You will be prompted for the filename. |
| down | Download: Downloads a specific file from the server to your local machine. |
| cmds | Help: Displays a list of all available commands supported by the protocol. |
| q / quit | Exit: Gracefully closes the connection and shuts down the client. |

---

## 3. Understanding the Transfer Process

The system uses a Dual-Channel approach. When you initiate an `up` or `down` command, the following occurs:

**Handshake:** The client sends a request over the Control Port (8080).  
**Verification:** The server checks if the file exists (for downloads) or prepares the directory (for uploads).  
**Data Stream:** A temporary thread is spawned on the Data Port (8081) to handle the actual binary transfer. This keeps the Control Port free for other commands (for example, running `ls` during a long upload).  
**Completion:** Once the bytes are received, the data thread closes and the server logs the transaction.

---

## 4. Reviewing Logs

You can monitor the "behind-the-scenes" actions by checking the `logs/` directory:

- `controlconnection.txt`  
  Records every command sent and every client connection.

- `dataconnection.txt`  
  Records the status of file transfers, including byte counts and any errors that occur during streaming.

---

## 5. Troubleshooting

**Permission Denied:**  
Ensure the application has permission to write to the `files/` and `logs/` folders.

**Connection Refused:**  
Ensure the server is running before you start the client.

**File Not Found:**  
- For uploads: ensure the file is in the same directory as the client executable.  
- For downloads: ensure the file exists in the server's `files/` directory.

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