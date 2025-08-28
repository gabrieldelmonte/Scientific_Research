use std::time::{Duration, Instant};
use std::io::{self, Read, Write};

fn main() -> io::Result<()> {
    let port_name = "/dev/ttyS4";
    let baud_rate = 9600;

    let mut port = serialport::new(port_name, baud_rate)
        .timeout(Duration::from_millis(500))
        .open()
        .expect("Não foi possível abrir a UART4");

    println!("BeagleBone: Iniciando teste de echo na UART4...");

    let mut buffer = [0u8; 1];
    let mut last_received = Instant::now();

    loop {
        match port.read(&mut buffer) {
            Ok(1) => {
                let received = buffer[0];
                println!("BeagleBone recebeu: {}", received as char);

				port.write_all(&[received])?;
                println!("BeagleBone enviou (echo): {}", received as char);

                last_received = Instant::now();
            }
            _ => {
                if last_received.elapsed() >= Duration::from_secs(5) {
                    port.write_all(&[b'1'])?;
                    println!("BeagleBone não recebeu nada, enviou: 1");
                    last_received = Instant::now();
                }
            }
        }
    }
}
