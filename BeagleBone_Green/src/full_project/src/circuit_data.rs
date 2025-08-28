/*
	circuit_data.rs
*/

#[allow(non_camel_case_types)]
#[derive(Clone, Debug)]
pub struct Circuit_Data {
	pub circuit_state: String,
	pub expected_voltage: f64,
	pub circuit_voltage: f64,
	pub circuit_current: f64,
	pub circuit_uptime: String,
	pub received_timestamp: String,
	pub insertion_timestamp: String
}
