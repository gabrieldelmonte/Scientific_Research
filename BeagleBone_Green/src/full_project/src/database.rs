/*
	database.rs
*/

use crate::circuit_data::Circuit_Data;

use rusqlite::{
	Connection,
	Result,
	params
};

use std::sync::{
	Arc,
	Mutex
};

use chrono::Local;

use std::thread;

use std::time::Duration;

#[derive(Debug)]
pub struct Database {
	connection: Connection
}

impl Database {
	pub fn new(database_path: &str) -> Result<Self> {
		let connection = Connection::open(database_path)?;

		connection.execute(
			"CREATE TABLE IF NOT EXISTS circuit_data (
				id					INTEGER PRIMARY KEY AUTOINCREMENT,
				circuit_state		TEXT NOT NULL,
				expected_voltage	REAL NOT NULL,
				circuit_voltage		REAL NOT NULL,
				circuit_current		REAL NOT NULL,
				circuit_uptime		TEXT NOT NULL,
				received_timestamp	TEXT NOT NULL,
				insertion_timestamp	TEXT NOT NULL
			)",
			[]
		)?;

		println!("[INFO] Database initialized at: '{}'!", database_path);

		Ok(Database { connection })
	}

	pub fn insert_data(&self, data: &Circuit_Data) -> Result<()> {
		self.connection.execute(
			"INSERT INTO circuit_data (circuit_state, expected_voltage, circuit_voltage, circuit_current, circuit_uptime, received_timestamp, insertion_timestamp)
			VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7)",
			params![
				data.circuit_state,
				data.expected_voltage,
				data.circuit_voltage,
				data.circuit_current,
				data.circuit_uptime,
				data.received_timestamp,
				data.insertion_timestamp
			]
		)?;

		Ok(())
	}

	pub fn get_last_n_data(&self, n: i64) -> Result<Vec<(i64, Circuit_Data)>> {
		let mut statement = self.connection.prepare(
			"SELECT id, circuit_state, expected_voltage, circuit_voltage, circuit_current, circuit_uptime, received_timestamp, insertion_timestamp
			FROM circuit_data
			ORDER BY id DESC
			LIMIT ?1"
		)?;

		let rows = statement.query_map([n], |row| {
			Ok((
				row.get::<_, i64>(0)?,
				Circuit_Data {
					circuit_state: row.get(1)?,
					expected_voltage: row.get(2)?,
					circuit_voltage: row.get(3)?,
					circuit_current: row.get(4)?,
					circuit_uptime: row.get(5)?,
					received_timestamp: row.get(6)?,
					insertion_timestamp: row.get(7)?
				}
			))
		})?;

		let mut results = Vec::new();
		for row in rows {
			results.push(row?);
		}

		Ok(results)
	}

	pub fn start(database_path: String, shared_data: Arc<Mutex<Circuit_Data>>, delay_insertion: u8) {
		std::thread::spawn(move || {
			let thread_database = Database::new(&database_path).expect("[WARN] Failed to open database in thread!");
			loop {
				let mut data = shared_data
					.lock()
					.unwrap()
					.clone();

				data.insertion_timestamp = Local::now().format("%Y-%m-%d_%H:%M:%S").to_string();

				if let Err(error) = thread_database.insert_data(&data) {
					eprintln!("[WARN] Failed to insert data: {:?}!", error);
				}

				thread::sleep(Duration::from_secs(delay_insertion.into()));
			}
		});
	}
}
