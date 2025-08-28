/*
	uart.rs
*/

use crate::Circuit_Data;

use std::sync::{
	Arc,
	Mutex
};

use chrono::Local;

use std::io::Read;

use std::time::Duration;

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub struct UART_Port {
	port_name: String,
	baud_rate: u32
}

impl UART_Port {
	pub fn new(port_name: &str, baud_rate: u32) -> Self {
		UART_Port {
			port_name: port_name.to_string(),
			baud_rate
		}
	}

	pub fn start(self, shared_data: Arc<Mutex<Circuit_Data>>) {
		std::thread::spawn(move || {
			let mut uart_port = serialport::new(&self.port_name, self.baud_rate)
				.timeout(Duration::from_secs(1))
				.open()
				.expect("Could not open UART port!");

			println!("[INFO] UART '{}' initialized at '{}' baud!", self.port_name, self.baud_rate);

			let mut buffer = [0u8; 32];
			loop {
				match uart_port.read(&mut buffer) {
					Ok(size) if size > 0 => {
						// NOTE: this code assumes that the
						// UART will receive data as follows:
						// 12.5,12.7,108.5,472
						// <expected_voltage>,
						// <circuit_voltage>
						// <circuit_current>
						// <circuit_uptime_secs>
						if let Ok(message) = std::str::from_utf8(&buffer[..size]) {
							let parts: Vec<&str> = message.trim().split(",").collect();
							if parts.len() == 2 {
								let (
									Ok(_),
									Ok(message_circuit_uptime)
								) = (
									parts[0].parse::<String>(),
									parts[1].parse::<String>()
								);
								let mut data = shared_data.lock().unwrap();
								data.circuit_state = "OFF".to_string();
								data.expected_voltage = 0.0;
								data.circuit_voltage = 0.0;
								data.circuit_current = 0.0;
								data.circuit_uptime = message_circuit_uptime;
								data.received_timestamp = Local::now().format("%Y-%m-%d_%H:%M:%S").to_string();
								data.insertion_timestamp = Local::now().format("%Y-%m-%d_%H:%M:%S").to_string();
							}
							else if parts.len() == 4 {
								if let (
									Ok(message_expected_voltage),
									Ok(message_circuit_voltage),
									Ok(message_circuit_current),
									Ok(message_circuit_uptime)
								) = (
									parts[0].parse::<f64>(),
									parts[1].parse::<f64>(),
									parts[2].parse::<f64>(),
									parts[3].parse::<String>()
								) {
									let mut data = shared_data.lock().unwrap();
									data.circuit_state = "ON".to_string();
									data.expected_voltage = message_expected_voltage;
									data.circuit_voltage = message_circuit_voltage;
									data.circuit_current = message_circuit_current;
									data.circuit_uptime = message_circuit_uptime.to_string();
									data.received_timestamp = Local::now().format("%Y-%m-%d_%H:%M:%S").to_string();
									data.insertion_timestamp = Local::now().format("%Y-%m-%d_%H:%M:%S").to_string();
								}
							}
							println!("[INFO] Message received: {}", message);
						}
					}
					_ => {
						//
					}
				}
				buffer = [0u8; 32];
			}
		});
	}
}
