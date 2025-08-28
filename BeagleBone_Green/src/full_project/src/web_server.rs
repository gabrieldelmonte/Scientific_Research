/*
	web_server.rs
*/

use crate::Circuit_Data;
use crate::Database;

use axum::{
	Router,
	response::Html,
	routing::get
};

use std::thread;

use std::net::SocketAddr;

use std::sync::Arc;

use tokio::runtime::Runtime;

pub struct WebServer{
	//
}

impl WebServer {
	async fn handler(database_path: Arc<String>, table_size: u8) -> Html<String> {
		let database_handler = Database::new(&database_path)
			.expect("[WARN] Failed to open database inside web server handler");

		println!("[INFO] Database inside web server handler initialized at: '{:?}!'", database_handler);

		let recent_data = database_handler
			.get_last_n_data(table_size.into())
			.expect("[WARN] Failed to fetch data from database!");

		let (id,
			Circuit_Data {
				circuit_state,
				expected_voltage,
				circuit_voltage,
				circuit_current,
				circuit_uptime,
				received_timestamp,
				insertion_timestamp
			}) =
			recent_data
				.first()
				.cloned()
				.unwrap_or((
					0,
					Circuit_Data {
						circuit_state: "-".into(),
						expected_voltage: 0.0,
						circuit_voltage: 0.0,
						circuit_current: 0.0,
						circuit_uptime: "-".into(),
						received_timestamp: "-".into(),
						insertion_timestamp: "-".into()
					}
				));

		let recent_rows: String = recent_data.iter().map(| (id, Circuit_Data {
			circuit_state,
			expected_voltage,
			circuit_voltage,
			circuit_current,
			circuit_uptime,
			received_timestamp,
			insertion_timestamp
			}) | {
			format!(
				r#"
					<tr>
						<td>{id}</td>
						<td>{circuit_state}</td>
						<td>{expected_voltage:.2} V</td>
						<td>{circuit_voltage:.2} V</td>
						<td>{circuit_current:.2} mA</td>
						<td>{circuit_uptime}</td>
						<td>{received_timestamp}</td>
						<td>{insertion_timestamp}</td>
					</tr>
				"#
			)
		}).collect();

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
							<td>Circuit State</td>
							<td>{circuit_state}</td>
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
							<td>Circuit Uptime</td>
							<td>{circuit_uptime}</td>
						</tr>
						<tr>
							<td>Received Time</td>
							<td>{received_timestamp}</td>
						<tr>
							<td>Insertion Time</td>
							<td>{insertion_timestamp}</td>
						</tr>
					</table>

					<h2>Recent Data</h2>
					<table>
						<tr>
							<th>ID</th>
							<th>Circuit State</th>
							<th>Expected Voltage</th>
							<th>Circuit Voltage</th>
							<th>Circuit Current</th>
							<th>Circuit Uptime</th>
							<th>Received Time</th>
							<th>Insertion Time</th>
						</tr>
						{recent_rows}
					</table>
				</body>
				</html>
			"#
		);

		println!("[INFO] Web server HTML build completed successfully!");

		Html(html)
	}

	pub fn start(database_path: String, table_size: u8) {
		thread::spawn(move || {
			let runtime = Runtime::new().unwrap();

			runtime.block_on(async {
				let database_arc = Arc::new(database_path);

				let app = Router::new().route("/", get(move || {
						let db = Arc::clone(&database_arc);
						async move { Self::handler(db, table_size).await }
					})
				);

				let address = SocketAddr::from(([0, 0, 0, 0], 38534));

				let listener = tokio::net::TcpListener::bind(address).await.unwrap();

				println!("[INFO] Web server running at: 'http://{}'!", address);

				axum::serve(listener, app)
					.await
					.unwrap();
			});
		});
	}
}
