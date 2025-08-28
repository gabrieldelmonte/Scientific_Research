mod circuit_data;
mod database;
mod lcd_display_impl;
mod uart;
mod web_server;

use std::sync::{
	Arc,
	Mutex
};

use circuit_data::Circuit_Data;
use database::Database;
use lcd_display_impl::LCD_Display_impl;
use uart::UART_Port;
use web_server::WebServer;

use lcd_display::{
	gpio_pin::GPIO_Pin,
	lcd::LCD_Mode
};

fn main() {
	// Shared data
    let shared_data = Arc::new(Mutex::new(Circuit_Data {
			circuit_state: "OFF".to_string(),
			expected_voltage: 0.0,
			circuit_voltage: 0.0,
			circuit_current: 0.0,
			circuit_uptime: "-".to_string(),
			received_timestamp: "-".to_string(),
			insertion_timestamp: "-".to_string()
		}));


	// Database path
	let database_path = "database/circuit.db".to_string();


	// Database
	let delay_insertion: u8 = 5;
	Database::start(database_path.clone(), Arc::clone(&shared_data), delay_insertion);


	// LCD
	let rs = GPIO_Pin::new("/dev/gpiochip1", 03).expect("Error while opening RS pin!");
	let en = GPIO_Pin::new("/dev/gpiochip1", 04).expect("Error while opening EN pin!");

	let d4 = GPIO_Pin::new("/dev/gpiochip0", 12).expect("Error while opening D4 pin!");
	let d5 = GPIO_Pin::new("/dev/gpiochip3", 26).expect("Error while opening D5 pin!");
	let d6 = GPIO_Pin::new("/dev/gpiochip0", 14).expect("Error while opening D6 pin!");
	let d7 = GPIO_Pin::new("/dev/gpiochip1", 01).expect("Error while opening D7 pin!");

	let data_pins = vec![
		d4,
		d5,
		d6,
		d7
	];

	let columns: u8 = 20;
	let rows: u8 = 4;

	let lcd_display = LCD_Display_impl::new(
		rs,
		en,
		data_pins,
		LCD_Mode::FourBit,
		columns,
		rows
	);

	let delay_update: u8 = 2;
	lcd_display.start(Arc::clone(&shared_data), delay_update);


	// UART
	let uart_port_name: String = "/dev/ttyS4".to_string();
	let uart_baud_rate: u32 = 9600;
	let uart_port = UART_Port::new(&uart_port_name, uart_baud_rate);
	uart_port.start(Arc::clone(&shared_data));


	// Web server
	let table_size: u8 = 30;
	WebServer::start(database_path.clone(), table_size);


	// Main loop...
	loop {
		std::thread::sleep(std::time::Duration::from_secs(10));
		println!();
		println!("[INFO] main.rs watchdog still alive!");
		println!();
	}
}
