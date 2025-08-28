extern crate libbeaglebone;

use libbeaglebone::prelude::*;
use std::time::Duration;

fn main() {
    // Abre UART4
    let mut uart_port = UART::new(4).unwrap();

    // Configura baud e timeout
    let _ = uart_port.set_baud_rate(serialport::BaudRate::Baud9600);
    let _ = uart_port.set_timeout(Duration::from_millis(500));

    println!("Echo test UART4 iniciado...");

    loop {
        // Envia string
        uart_port.write("Hello UART4!\n").unwrap();

        // Lê até 64 bytes
        match uart_port.read_to_string(64) {
            Ok(s) if !s.is_empty() => println!("Recebido: {}", s),
            Ok(_) => println!("Nada recebido"),
            Err(e) => eprintln!("Erro na leitura: {:?}", e),
        }

        std::thread::sleep(Duration::from_secs(1));
    }
}


/*
extern crate libbeaglebone;

use libbeaglebone::prelude::*;
use std::time::Duration;

fn main() {
    let mut uart_port = UART::new(2).unwrap();
    let _ = uart_port.set_baud_rate(serialport::BaudRate::BaudOther(9600));

    println!("Serial port: {:?}", uart_port.settings());

    let message = String::from("Juju");
    for index in 0..3 {
        for _ in 0..5 {
            let _ = uart_port.write(&message);
            let returned_message = uart_port.read_to_string(1);
            println!("Message: {:?} - Index: {:?}", returned_message, index);
        }
    }
}
*/
