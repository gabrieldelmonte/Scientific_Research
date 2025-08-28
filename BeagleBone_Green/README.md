# BeagleBone Green Project

This project is designed to monitor and display circuit data using a BeagleBone Green board. It integrates multiple components including LCD display, UART communication, web server, and database storage for comprehensive circuit monitoring.

## Project Structure

```
BeagleBone_Green/
├── README.md                       # This file
├── src/
│   ├── full_project/               # Main application
│   │   ├── Cargo.toml              # Dependencies configuration
│   │   ├── README.md               # Project-specific documentation
│   │   ├── database/
│   │   │   └── circuit.db          # SQLite database for storing circuit data
│   │   └── src/
│   │       ├── main.rs             # Main entry point
│   │       ├── circuit_data.rs     # Circuit data structure definitions
│   │       ├── database.rs         # Database operations and management
│   │       ├── lcd_display_impl.rs # LCD display implementation
│   │       ├── lib.rs              # Library module definitions
│   │       ├── uart.rs             # UART communication handler
│   │       └── web_server.rs       # Web server for data visualization
│   └── Tests/                      # Test applications and examples
│       ├── blinky_led/             # GPIO LED blinking test
│       ├── echo_test/              # UART echo test
│       ├── lcd_test/               # LCD display test
│       ├── uart_test/              # UART communication test (with libbeaglebone)
│       └── web_server/             # Web server standalone test
```

## Components Overview

### Main Application (`src/full_project/`)

The main application is a comprehensive circuit monitoring system that combines several subsystems:

#### Core Modules

- **`main.rs`**: Central orchestrator that initializes and starts all subsystems
- **`circuit_data.rs`**: Defines the shared data structure for circuit measurements
- **`database.rs`**: Handles SQLite database operations for persistent data storage
- **`lcd_display_impl.rs`**: Manages the 20x4 character LCD display for real-time data
- **`uart.rs`**: Handles UART communication for receiving circuit data
- **`web_server.rs`**: Provides HTTP interface for remote monitoring

#### Key Features

- **Real-time Data Display**: 20x4 LCD showing current circuit status
- **Data Persistence**: SQLite database for historical data storage
- **Web Interface**: HTTP server for remote monitoring and data visualization
- **UART Communication**: Receives data from external circuit monitoring devices
- **Multi-threaded Architecture**: Concurrent handling of all subsystems

### Test Applications (`src/Tests/`)

Collection of standalone test programs for validating individual components:

#### `blinky_led/`
- **Purpose**: GPIO functionality test
- **Function**: Blinks an LED connected to specified GPIO pin
- **Usage**: `cargo run -- <chip_path> <line_offset>`
- **Example**: `cargo run -- /dev/gpiochip3 5`

#### `echo_test/`
- **Purpose**: Basic UART echo test
- **Function**: Tests UART communication by echoing received data

#### `lcd_test/`
- **Purpose**: LCD display functionality test
- **Function**: Tests LCD initialization and display capabilities

#### `uart_test/`
- **Purpose**: Advanced UART testing with libbeaglebone
- **Function**: Tests UART4 communication with external devices
- **Features**: Configurable baud rate, timeout handling, read/write operations

#### `web_server/`
- **Purpose**: Standalone web server test
- **Function**: Independent web server for testing HTTP interface
- **Features**: 
  - SQLite database integration
  - HTML table generation
  - Real-time data simulation
  - Auto-refresh functionality

## Hardware Configuration

### LCD Display (20x4 Character)
- **RS**: GPIO1_03 (`/dev/gpiochip1`, pin 3)
- **EN**: GPIO1_04 (`/dev/gpiochip1`, pin 4)
- **D4**: GPIO0_12 (`/dev/gpiochip0`, pin 12)
- **D5**: GPIO3_26 (`/dev/gpiochip3`, pin 26)
- **D6**: GPIO0_14 (`/dev/gpiochip0`, pin 14)
- **D7**: GPIO1_01 (`/dev/gpiochip1`, pin 1)

### UART Communication
- **Port**: `/dev/ttyS4` (UART4)
- **Baud Rate**: 9600
- **Purpose**: Receives circuit measurement data

### Web Server
- **Port**: 38534
- **Interface**: All interfaces (0.0.0.0)
- **Purpose**: Remote monitoring and data visualization

## Dependencies

### Main Project Dependencies
- **lcd_display**: Custom LCD display library (v0.1.0)
- **serialport**: UART communication (v4.2)
- **tokio**: Async runtime with full features (v1.47.1)
- **rusqlite**: SQLite database with bundled features (v0.37.0)
- **axum**: Web framework (v0.8.4)
- **chrono**: Date and time handling (v0.4.41)

### Test Dependencies
- **gpio_cdev**: GPIO character device interface
- **libbeaglebone**: BeagleBone-specific hardware abstraction

## Getting Started

### Prerequisites
- Rust toolchain installed
- BeagleBone Green board
- Proper GPIO permissions
- Hardware connections as specified above

### Building and Running

1. **Main Application**:
   ```bash
   cd src/full_project
   cargo build --release
   sudo ./target/release/full_project
   ```

2. **Individual Tests**:
   ```bash
   cd src/Tests/<test_name>
   cargo run
   ```

### Web Interface Access
Once the main application is running, access the web interface at:
```
http://<beaglebone_ip>:38534
```

## Data Flow

1. **UART Reception**: Circuit data received via UART4
2. **Data Processing**: Parsed and stored in shared memory structure
3. **Database Storage**: Periodic insertion into SQLite database
4. **LCD Display**: Real-time display updates every 2 seconds
5. **Web Interface**: HTTP access to current and historical data

## Monitoring Features

- **Real-time Circuit Status**: ON/OFF state display
- **Voltage Monitoring**: Expected vs actual voltage readings
- **Current Measurement**: Circuit current in milliamps
- **Uptime Tracking**: Circuit operational duration
- **Historical Data**: Web-based access to last 30 measurements
- **Timestamping**: Automatic timestamp recording for all measurements

## Development Notes

- **Thread Safety**: Uses `Arc<Mutex<>>` for shared data access
- **Error Handling**: Comprehensive error handling for hardware interfaces
- **Modular Design**: Separated concerns for maintainability
- **Testing**: Individual test programs for each subsystem
- **Documentation**: Inline documentation for complex functions

## License

This project is part of the Scientific Research repository and follows the same licensing terms.
