# xTrainAPI - Unified Model Train Control

Welcome to the xTrainAPI project! This repository contains a C++-based API for controlling digital model trains and their accessories.

## Documentation

Our API documentation is automatically generated and hosted on GitHub Pages.

*   **Doxygen (C++ API):** Detailed documentation of the C++ interface, classes, and methods can be found here:
    [Doxygen Documentation](https://chatelao.github.io/xDuinoRails_xTrainAPI/doxygen)

*   **Swagger (REST API):** The specification for our REST interface, including interactive endpoints, is available here:
    [Swagger UI](https://chatelao.github.io/xDuinoRails_xTrainAPI/swagger)

*   **Web Tool:** A simple web application for interacting with the xTrainAPI is available here:
    [Web Tool](https://chatelao.github.io/xDuinoRails_xTrainAPI/webtool)

## What is xTrainAPI?

xTrainAPI is a unified interface that allows various digital model train systems to be controlled through a single, consistent API. This simplifies the development of control software by abstracting the complexity of individual protocols (such as DCC, Märklin Motorola, etc.).

## Core Concepts

The API is defined in C++ as an abstract interface (`IUnifiedModelTrainListener`) and serves as the "Single Source of Truth". All other artifacts, such as the XML schemas and the OpenAPI specification, are derived from this C++ header file.

For a deeper understanding of the terms and data fields used in the API, please take a look at our [Glossary](XTTRAIN_GLOSSARY.en.md) and the [Data Dictionary](DATA_DICTIONARY.en.md).

## Repository Structure

The repository is organized as follows:

-   `/xDuinoRails_xTrainAPI.h`: The main C++ API definition.
-   `/docs`: Contains additional documentation, such as the CLI syntax.
-   `/examples`: Arduino sketches demonstrating how to use the API.
-   `/json`: JSON schemas for the API events.
-   `/lib`: Contains the `yxml` library for XML parsing.
-   `/swagger`: The OpenAPI 3.0 specification for the REST API.
-   `/webtool`: A web-based tool for interacting with the API.
-   `/xml`: XML schemas and examples.

## Getting Started

To use the xTrainAPI in your own Arduino projects, follow these steps:

1.  **Include the Library:** Copy this entire repository into your Arduino `libraries` folder.
2.  **Implement the Interface:** Create a new class in your sketch that inherits from `IUnifiedModelTrainListener` and implement the methods for the events you want to handle.
3.  **Process Events:** In your `loop()` function, you can use one of the utility classes (like `CmdLineParser` or `XmlParser`) to parse incoming data and trigger the corresponding events in your listener class.

## Running the Examples

The `/examples` directory contains several sketches that demonstrate the functionality of the API. To run them, you will need to have the Arduino IDE or `arduino-cli` installed.

1.  Open one of the example sketches (e.g., `examples/debug_cli/debug_cli.ino`) in the Arduino IDE.
2.  Select your board and port.
3.  Upload the sketch to your Arduino.
4.  Open the Serial Monitor to interact with the example.

## Contributing

We welcome contributions! Please read our `AGENTS.md` before making any changes to the code.
