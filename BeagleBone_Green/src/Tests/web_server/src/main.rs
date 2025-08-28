use axum::{
	Router,
	response::Html,
	routing::get
};
use rusqlite::{
	Connection,
	params
};
use tokio::time::{
	Duration,
	sleep
};
use chrono::Local;
use std::net::SocketAddr;
use std::sync::Arc;
use std::fs;

fn init_db(database_path: &str) -> Connection {
	if let Some(parent) = std::path::Path::new(database_path).parent() {
		if let Err(error) = fs::create_dir_all(parent) {
			println!("Warning: could not create directory {:?}: {}", parent, error);
		}
	}

	println!("Database at: {}", database_path);
	let connection = Connection::open(&*database_path).unwrap();

	connection.execute(
		"CREATE TABLE IF NOT EXISTS circuit_data (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			expected_voltage REAL NOT NULL,
			circuit_voltage REAL NOT NULL,
			circuit_current REAL NOT NULL,
			execution_time TEXT NOT NULL,
			insertion_timestamp TEXT NOT NULL
		)",
		[],
	).unwrap();

	connection
}

async fn handler(database_path: Arc<String>) -> Html<String> {
	let connection = Connection::open(&*database_path)
		.expect("Failed to open database connection");

	let mut statement = connection.prepare(
		"SELECT id, expected_voltage, circuit_voltage, circuit_current, execution_time, insertion_timestamp
		FROM circuit_data ORDER BY id DESC LIMIT 30"
	).expect("Failed to prepare statement");

	let rows = statement.query_map([], |row| {
		Ok((
			row.get::<_, i64>(0)?,		// ID
			row.get::<_, f64>(1)?,		// expected_voltage
			row.get::<_, f64>(2)?,		// circuit_voltage
			row.get::<_, f64>(3)?,		// circuit_current
			row.get::<_, String>(4)?,	// execution_time
			row.get::<_, String>(5)?,	// insertion_timestamp
		))
	}).unwrap();

	let mut recent_data = Vec::new();
	for row in rows {
		recent_data.push(row.unwrap());
	}

	let (id, expected_voltage, circuit_voltage, circuit_current, execution_time, insertion_timestamp) = recent_data
		.first()
		.cloned()
		.unwrap_or((
			0,
			0.0,
			0.0,
			0.0,
			"-".into(),
			"-".into()
		));

	let recent_rows: String = recent_data.iter().map(|(id, ev, v, c, et, ct)| {
		format!(
			r#"
			<tr>
				<td>{id}</td>
				<td>{ev:.2} V</td>
				<td>{v:.2} V</td>
				<td>{c:.2} mA</td>
				<td>{et}</td>
				<td>{ct}</td>
			</tr>
			"#
		)
		})
		.collect();

	let html = format!(
		r#"
			<!DOCTYPE html>
			<html lang = "en">
			
			<head>
				<meta charset = "UTF-8">
				<title>BeagleBone Green - Circuit Data</title>
				<style>
					body {{
						font-family: Arial, sans-serif;
						margin: 40px;
						background-color: #F5F5F5;
					}}

					h1, h2 {{
						text-align: center;
						color: #333
					}}

					table {{
						margin: auto;
						border-collapse: collapse;
						width: 80%;
						background-color: #FFF;
						box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
						margin-bottom: 40px;
					}}

					th, td {{
						border: 1px solid #CCC;
						padding: 12px;
						text-align: center;
					}}

					th {{
						background-color: #E16419;
						color: white;
					}}

					tr:nth-child(even) {{
						background-color: #F2F2F2;
					}}
				</style>
			</head>

			<body>
				<h1>Circuit Data</h1>
				<table>
					<tr>
						<th>Parameter</th>
						<th>Value</th>
					</tr>
					<tr>
						<td>ID</td>
						<td>{id}</td>
					</tr>
					<tr>
						<td>Expected Voltage</td>
						<td>{expected_voltage} V</td>
					</tr>
					<tr>
						<td>Circuit Voltage</td>
						<td>{circuit_voltage} V</td>
					</tr>
					<tr>
						<td>Circuit Current</td>
						<td>{circuit_current} mA</td>
					</tr>
					<tr>
						<td>Execution Time</td>
						<td>{execution_time}</td>
					</tr>
					<tr>
						<td>Insertion time</td>
						<td>{insertion_timestamp}</td>
					</tr>
				</table>

				<h2>Recent Data</h2>
				<table>
					<tr>
						<th>ID</th>
						<th>Expected Voltage</th>
						<th>Circuit Voltage</th>
						<th>Circuit Current</th>
						<th>Execution Time</th>
						<th>Insertion Time</th>
					</tr>
					{recent_rows}
				</table>
			</body>
			</html>
		"#
	);

	Html(html)
}

async fn data_inserter(database_path: Arc<String>) {
	let connection = Connection::open(&*database_path).unwrap();
	let mut counter: i64 = connection
		.query_row("SELECT COALESCE(MAX(id), 0) FROM circuit_data", [], |row| row.get(0))
		.unwrap();

	loop {
		counter += 1;
		
		// Use a format that SQLite won't interpret as datetime
		let timestamp_string = format!("{}", Local::now().format("%Y-%m-%d_%H:%M:%S"));

		connection.execute(
			"INSERT INTO circuit_data (expected_voltage, circuit_voltage, circuit_current, execution_time, insertion_timestamp)
			VALUES (?1, ?2, ?3, ?4, ?5)",
			params![
				12.5,
				12.7 + (counter as f64 * 0.01),
				108.1 + (counter as f64 * 0.1),
				format!("00:06:{:02}", counter),
				timestamp_string.clone()
			],
		).unwrap();

		sleep(Duration::from_secs(10)).await;
	}
}

#[tokio::main]
async fn main() {
	let database_path = Arc::new(String::from("database/circuit.db"));

	let _ = init_db(&database_path);

	let database_clone = Arc::clone(&database_path);
	tokio::spawn(async move { data_inserter(database_clone).await });

	sleep(Duration::from_secs(2)).await;

	let database_for_handler = Arc::clone(&database_path);
	let app = Router::new().route("/", get(move || {
		let db = Arc::clone(&database_for_handler);
		async move { handler(db).await }
	}));

	let addr = SocketAddr::from(([0, 0, 0, 0], 38534));

	println!("Server running: http://{}", addr);

	axum::serve(tokio::net::TcpListener::bind(addr).await.unwrap(), app)
		.await
		.unwrap();
}
