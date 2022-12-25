## System Diagram

<link href='https://raw.githubusercontent.com/MaSiRoProjectOSS/CANDataViewer/master/doc/style.css' rel='stylesheet' type='text/css' media='all'>


## Overall view

The system is shown below.

```plantuml
@startuml
rectangle system  as "board" {
cloud WebServer
rectangle board as "ATOM Lite" {
  rectangle SPPIFS as "FLASH"
}
rectangle can_device_A


}
rectangle can_device_B
actor browser

browser -right- WebServer : WiFi\n(2.4GHz)
WebServer -right- board
board -down- can_device_A : "SPI\nor\nGrove"
can_device_A .down. can_device_B : CAN

@enduml
```

## Mode State

The mode changes as follows.

```plantuml
@startuml
title "Mode State"

NOT_INITIALIZE : bool setup_default(void)
FINISHED : --
state Active {
  READY : bool send_for_ready(CanData *data)
  RUNNING : bool send_for_running(CanData *data)
  STOPPING : bool send_for_stopping(CanData *data)
  INACTIVE : --
  ABORT : --
  PAUSE : --
[*] -[dashed]right-> READY
}

[*] -[dashed]right-> NOT_INITIALIZE : begin();

NOT_INITIALIZE -[dashed]right-> Active : request_start()
READY --> RUNNING : request_run()
RUNNING -[dotted]-> ABORT : abort()
INACTIVE --> READY : request_resume()
ABORT --> READY : request_resume()

RUNNING --> STOPPING : request_suspend()
STOPPING --> INACTIVE : request_sleep()
RUNNING --> RUNNING : loop()
RUNNING -[dotted]-> PAUSE : request_pause()
PAUSE --> RUNNING : request_resume()
PAUSE --> STOPPING : request_pause()


Active -[dashed]-> FINISHED : request_stop()
FINISHED -[dashed]right-> [*]

@enduml
```
