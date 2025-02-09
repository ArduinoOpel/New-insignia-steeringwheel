# New-insignia-steeringwheel
cruiscontrol over resistor lader en radio buttens read from Linbus en write to Canbus

Hardware: Linbus: TTL UART to LIN-bus converter
Canbus: mcp2515 module

Arduino: Arduino Nano Power supply: cigarette lighter print
Relays: 5x 5v relays
Resistors: See diagram

Connect Linbus serial to: A1 and A0
Connect Canbus according to diagram
Connect resistor ladder control to A5 and a 5V power supply from Arduino Nano (note: pull-down resistor on A5.
Connect 5x relays to Arduino Nano (see sketch)


