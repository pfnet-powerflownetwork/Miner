# PFNET Miner

## Overview
The **PFNET Miner** is a core component of the Power Flow Network (PFNET) project. It is responsible for executing distributed computational tasks by participating as a node in the decentralized network. The miner handles task submissions, validates solutions, and secures the network using Proof of Work (PoW) mechanisms.

## Features
- Decentralized task execution
- Integration with the PFNET blockchain for task validation
- Support for bounty and PoW solutions
- Written in C for high performance

## Repository Structure
```
Miner/
├── PFNETPL/          # Platform Layer for different computing devices
├── crypto/           # Cryptographic utilities
├── docker/           # Docker setup for containerized deployment
├── work/             # Task handling and processing
├── CMakeLists.txt    # Build configuration
├── README.md         # Project documentation (this file)
├── miner.h           # Header file defining core miner functionalities
├── pfnet_miner.c     # Main implementation of the miner
└── util.c            # Utility functions for the miner
```

## Prerequisites
- C compiler (GCC or Clang)
- OpenSSL for cryptographic functions
- Docker (optional, for containerized deployment)

## Installation
### Step 1: Clone the Repository
```bash
git clone https://github.com/pfnet-powerflownetwork/Miner.git
cd Miner
```

### Step 2: Build the Miner
```bash
mkdir build
cd build
cmake ..
make
```

## Usage
### Running the Miner
After building the project, you can run the miner with the following command:
```bash
./pfnet_miner
```

### Docker Deployment
To run the miner in a Docker container:
```bash
docker build -t pfnet-miner .
docker run -d pfnet-miner
```

## Contributing
Contributions are welcome! Please follow the standard GitHub workflow:
1. Fork the repository
2. Create a new branch
3. Commit your changes
4. Open a pull request


## Acknowledgments
- The PFNET team for their continuous efforts in decentralized computing.
- Contributors to open-source libraries used in this project.
