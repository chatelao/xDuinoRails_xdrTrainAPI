# xDuinoRails Arduino Examples

This directory contains example Arduino sketches that demonstrate how to use the `xDuinoRails_xTrainAPI`.

## Examples

* **debug_cli**: A simple command-line interface that allows you to send DCC-EX like commands to control a model train layout. This sketch acts as the test target.
* **print_monitor**: A sketch that demonstrates how to implement the `IUnifiedModelTrainListener` to receive events from the `xDuinoRails_xTrainAPI` and print them to the serial port in a DCC-EX like command format. This sketch acts as the test driver.

## End-to-End Testing

These examples are designed to be used together for end-to-end testing. The `print_monitor` sketch can be used to generate a command stream that can be piped to the `debug_cli` sketch. The output of the `debug_cli` sketch can then be compared to the original test data to verify that the commands are being parsed and handled correctly.

### Test Data

The `testdata` directory contains test data files that can be used for testing. Each file contains a sequence of commands that will be executed by the `print_monitor` sketch.

* `test_set_1.txt`
* `test_set_2.txt`

### Usage

1.  **Configure `print_monitor`:** Open `print_monitor.ino` and change the `TEST_SET` macro to the desired test set (1 or 2).
2.  **Upload Sketches:** Upload the `print_monitor` sketch to one Arduino and the `debug_cli` sketch to another.
3.  **Connect Arduinos:** Connect the TX pin of the `print_monitor` Arduino to the RX pin of the `debug_cli` Arduino. Also connect the grounds of the two boards.
4.  **Verify Output:** Open the Serial Monitor for the `debug_cli` Arduino. The output should be a series of API call logs. Compare this output to the commands in the corresponding test data file to verify that the commands were received and parsed correctly.
