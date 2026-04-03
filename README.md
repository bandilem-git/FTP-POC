# FTP: Proof Of Concept

This repo is a 'FTP **Proof Of Concept**' project.   
The aim of this project is to resimulate basic FTP over a network   

*this is a component for a bigger project*

## REQUIREMENTS
> compiler : clang++  
> C++ version: c++ 17 

## BUILDING PROJECT
> make all    
> make run-servers    
> make run client 

## CLEANING THE PROJECT
> make clean

## DESCRIPTION
This repo is a recreation of an FTP server not following all the RFC's but follows general convention 
user can upload file, download files and review logs

### CURRENT FEATURES
- Uploading Files: up + follow instructions
- Downloading Files: down + follow instructions
- Concurrent Uploading and Downloading
- Dual Channel usage


### FUTURE FEATURES