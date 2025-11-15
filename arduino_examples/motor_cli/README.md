# Motor CLI Example

This example demonstrates how to control a motor using the `XDuinoRails_MotorDriver` library with a simple command-line interface. It uses the `CmdLineParser` from the xTrainAPI.

## Command

The CLI accepts `THROTTLE` commands in the following format:

`THROTTLE cab="<address>" speed="<speed>" direction="<0|1>"`

- `<address>`: The address of the motor (e.g., 1).
- `<speed>`: The target speed (0-100).
- `<direction>`: 0 for reverse, 1 for forward.
