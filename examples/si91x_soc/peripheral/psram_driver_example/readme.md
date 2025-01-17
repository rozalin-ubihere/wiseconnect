# PSRAM Driver Example

## Table of Contents

- [Purpose/Scope](#purposescope)
- [Prerequisites/Setup Requirements](#prerequisitessetup-requirements)
  - [Hardware Requirements](#hardware-requirements)
  - [Software Requirements](#software-requirements)
  - [Setup Diagram](#setup-diagram)
- [Getting Started](#getting-started)
- [Application Build Environment](#application-build-environment)
- [PSRAM Core Component Installation](#psram-core-component-installation)
- [Test the Application](#test-the-application)

## Purpose/Scope

- This example project demonstrates use of the PSRAM Driver APIs via QSPI interface. The example writes and reads data to and from PSRAM using auto mode, manual blocking mode and DMA mode. The application makes use of default configurations of PSRAM device with fast quad read and write in QPI (Quad IO) interface mode.

## Prerequisites/Setup Requirements

### Hardware Requirements

- Windows PC
- Silicon Labs Si917 Evaluation Kit [WPK(BRD4002) + BRD4342A]

### Software Requirements

- Simplicity Studio
- Serial console Setup
  - The Serial Console setup instructions are provided below:
Refer [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/getting-started-with-soc-mode#perform-console-output-and-input-for-brd4338-a).

### Setup Diagram

> ![Figure: Introduction](resources/readme/setupdiagram.png)

## Getting Started

Refer to the instructions [here](https://docs.silabs.com/wiseconnect/latest/wiseconnect-getting-started/) to:

- Install Studio and WiSeConnect 3 extension
- Connect your device to the computer
- Upgrade your connectivity firmware
- Create a Studio project

## Application Build Environment

- PSRAM Read-Write type, interface mode, and operation frequency can be configured from "PSRAM Core" component under "Software Components" tab via GUI.

## PSRAM Core Component Installation

1. Open the .slcp file generated for your specific project by double-clicking it.
2. Navigate to the SOFTWARE COMPONENTS tab within the interface.
3. Utilize the search bar to look for "PSRAM."
4. Install the PSRAM core component and configure the necessary device configs

> ![Figure: Core Component Installation](resources/readme/core_component.png)
- Configuration for Pinset
> ![Figure: PSRAM Pin Configuration](resources/readme/pin_configs.png)
- Configuration for Clock, Read-Write type, Interface mode
> ![Figure: PSRAM Device Configuration](resources/readme/device_config.png)
  
## Test the Application

1. Compile and run the application.
2. Logs are printed with success or failure status for PSRAM read & write events in auto mode, manual mode, and manual via DMA mode.
3. Serial console output will be below.

    > ![Figure: Introduction](./resources/readme/outputConsole_PSRAM_DRIVER_Example.png)

