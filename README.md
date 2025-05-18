# Model-Based Design and Implementation of ARQ Protocol - Setup Guide
This project demonstrates a model-based approach to designing and implementing the Stop-and-Wait Automatic Repeat reQuest (ARQ) protocol. Utilizing tools like Simulink Stateflow and itemis CREATE, we modeled the protocol's behavior and generated corresponding C++ code. This code was then integrated into a Qt-based chat application to simulate reliable data transmission over potentially unreliable channels.

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Design Details](#design-details)
- [Contributors](#contributors)
- [License](#license)

## Overview

### Objectives

- **Modeling**: Design the Stop-and-Wait ARQ protocol using state machine modeling tools.
- **Code Generation**: Automatically generate C++ code from the models.
- **Integration**: Develop a Qt-based chat application that utilizes the generated code to ensure reliable communication.

### Tools & Technologies

- **Modeling Tools**:
  - Simulink Stateflow
  - itemis CREATE
- **Programming Languages**:
  - C++
- **Frameworks**:
  - Qt (for GUI and application logic)

## Project Structure

The repository is organized as follows:

```
Model-based-Design-and-Implementation-of-ARQ-Protocol/
├── Simulink/             # Simulink Stateflow models and their Qt-based chat application integration
├── itemisCREATE/         # itemis CREATE model and its Qt-based chat application integration
└── README.md             # Project overview and instructions
```

## Getting Started

### Prerequisites

- **Simulink Stateflow**: For viewing and editing the Stateflow models.
- **itemis CREATE**: For working with itemis CREATE models.
- **Qt Framework**: To build and run the chat application.
- **C++ Compiler**: Compatible with the Qt framework.

### Building the Chat Application

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Aminho09/Model-based-Design-and-Implementation-of-ARQ-Protocol.git
   ```

2. **Navigate to the Qt-based Chat Application**:
   - Change the current directory to either chat application implementations corresponding to each model designed:
   ```bash
   cd Simulink/User1/implementation
   cd Simulink/User2/implementation
   cd itemisCREATE/implementation
   ```

3. **Open the Project**:
   - Use Qt Creator to open the `.pro` file located in the `QtChatApplication` directory.

4. **Build the Project**:
   - Configure the project with the appropriate kit and build the application.

5. **Run the Application**:
   - Execute the built application to start the chat interface.
  
## Usage

1. **Launch the Application**:
   - Start the chat application on two separate instances or machines.

2. **Establish Connection**:
   - Use the application's interface to connect the two instances, simulating a sender and a receiver.

3. **Send Messages**:
   - Type and send messages from one instance; the Stop-and-Wait ARQ protocol ensures reliable delivery to the other instance.

## Design Details

### Stop-and-Wait ARQ Protocol

The Stop-and-Wait ARQ protocol is a fundamental method for error control in data transmission. It operates by sending one frame at a time and waiting for an acknowledgment before sending the next frame. If an acknowledgment isn't received within a specified timeout, the frame is retransmitted. This ensures reliable communication over unreliable or noisy channels.

### Modeling Approach

- **Simulink Stateflow**:
  - Utilized to create a visual state machine representing the ARQ protocol's behavior.
  - Includes two concurrent states `send` and `receive` doing send and receive behaviors.
  - Transitions are based on events like `send_data`, `send_ack`, `receive_packet`, and `Timeout`.
  - Functions which calculate the headers and check pakcets and acknowledgements.
    You can see the statechart in the picture below:
    ![SimulinkChart](https://github.com/user-attachments/assets/51e39c33-bc9b-476c-a23f-ae771ff5d6b0)


- **itemis CREATE**:
  - Provided a modele-based approach to define the state machine.
  - Allowed for clear definitions of states, events, and transitions in a structured format.
