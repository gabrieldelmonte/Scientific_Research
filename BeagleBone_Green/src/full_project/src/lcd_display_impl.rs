/*
	lcd_display_impl.rs
*/

use crate::circuit_data::Circuit_Data;

use std::sync::{
	Arc,
	Mutex
};

use std::thread;

use std::time::Duration;

use lcd_display::{
	lcd::LCD_Mode,
	GPIO_Pin,
	LCD
};	

#[allow(non_camel_case_types)]
#[derive(Debug)]
pub struct LCD_Display_impl {
	lcd: LCD
}

impl LCD_Display_impl {
	pub fn new(rs:GPIO_Pin, en: GPIO_Pin, data_pins: Vec<GPIO_Pin>, mode: LCD_Mode, columns: u8, rows: u8) -> Self {
		let mut lcd = LCD::new(rs, en, data_pins, mode);
		lcd.begin(columns, rows);
		lcd.clear();

		println!("[INFO] LCD display initialized with '{}' columns and '{}' rows!",
			lcd.get_columns(),
			lcd.get_rows()
		);

		LCD_Display_impl { lcd }
	}

	pub fn start(mut self, shared_data: Arc<Mutex<Circuit_Data>>, delay_update: u8) {
		thread::spawn(move || {
			loop {
				let data = shared_data.lock().unwrap();

				let hours;
				let minutes;
				let seconds;

				if data.circuit_uptime != "-" {
					hours = &data.circuit_uptime[0..2];
					minutes = &data.circuit_uptime[3..5];
					seconds = &data.circuit_uptime[6..8];
				}
				else {
					hours = "00";
					minutes = "00";
					seconds = "00";
				}

				let line_01;
				let line_02;
				let line_03;
				let line_04;

				if data.circuit_state == "OFF".to_string() {
					line_01 = format!("[Exp. V]: [OFF]");
					line_02 = format!("[V]: [OFF]");
					line_03 = format!("[I]: [OFF]");
					line_04 = format!("[Time]: {}:{}:{}", hours, minutes, seconds);
				}
				else {
					line_01 = format!("[Exp. V]: {:.2} V", data.expected_voltage);
					line_02 = format!("[V]: {:.2} V", data.circuit_voltage);
					line_03 = format!("[I]: {:.2} mA", data.circuit_current);
					line_04 = format!("[Time]: {}:{}:{}", hours, minutes, seconds);
				}

				if self.lcd.get_rows() == 2 {
					self.lcd.clear();

					self.lcd.set_cursor(0, 0);
					self.lcd.print(&line_01);

					self.lcd.set_cursor(0, 1);
					self.lcd.print(&line_02);

					thread::sleep(Duration::from_secs(delay_update.into()));

					self.lcd.clear();

					self.lcd.set_cursor(0, 0);
					self.lcd.print(&line_03);

					self.lcd.set_cursor(0, 1);
					self.lcd.print(&line_04);

					thread::sleep(Duration::from_secs(delay_update.into()));
				}
				else if self.lcd.get_rows() == 4 {
					self.lcd.clear();

					self.lcd.set_cursor(0, 0);
					self.lcd.print(&line_01);

					self.lcd.set_cursor(0, 1);
					self.lcd.print(&line_02);

					self.lcd.set_cursor(0, 2);
					self.lcd.print(&line_03);

					self.lcd.set_cursor(0, 3);
					self.lcd.print(&line_04);

					thread::sleep(Duration::from_secs(delay_update.into()));
				}
				else {
					println!("[INFO] This code supports only 2-row or 4-row LCDs. Please adjust it to match your display.");
				}
			}
		});
	}
}
