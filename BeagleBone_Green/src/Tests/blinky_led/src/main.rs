use gpio_cdev::{Chip, LineRequestFlags};
use std::{env, process, thread, time::Duration};

fn main() -> Result<(), Box<dyn std::error::Error>> {
	let args: Vec<String> = env::args().collect();

	if args.len() != 3 {
		eprintln!("Usage: cargo run -- <chip_path> <line_offset>");
		eprintln!("Example: cargo run -- /dev/gpiochip3 5");
		process::exit(1);
	}

    let chip_path = &args[1];
    let line_offset: u32 = args[2]
		.parse()
		.unwrap_or_else(|_| {
			eprintln!("Error: line_offset should be an integer!");
			process::exit(1);
		});

    let mut chip = match Chip::new(chip_path) {
        Ok(ok) => ok,
        Err(error) => {
            eprintln!("Error while opening GPIO chip {}! Error: {}", chip_path, error);
            process::exit(1);
        }
    };

    let handle = chip
        .get_line(
            line_offset
        )?
        .request(
            LineRequestFlags::OUTPUT,
            0,
            "blinky_led"
        )?;

    println!("By now, the GPIO {:#?} with the line offset of {} will blinky every 1 second!", chip, line_offset);
	println!();
	println!("To stop, just press Ctrl+C!");

    loop {
        if let Err(error) = handle.set_value(1) {
            eprintln!("Error setting GPIO {:#?} with the line offset of {} to high level! Error: {}", chip, line_offset, error)
        }
        thread::sleep(Duration::from_millis(1000));

        if let Err(error) = handle.set_value(0) {
            eprintln!("Error setting GPIO {:#?} with the lien offset of {} to low level! Error: {}", chip, line_offset, error)
        }
        thread::sleep(Duration::from_millis(1000));
    }
}
