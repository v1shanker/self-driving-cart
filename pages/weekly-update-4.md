---
layout: default
title: Self-Driving Cart
tagline: Team C9 Project Website
description: Minimal tutorial on making a simple website with GitHub Pages
---

# Weekly Update 4 (4/2 - 4/9)

**Vikram**

+ Spent a lot of time debugging the chassis. Came to the conclusion that the
  fuse bits are incorrectly programmed.
+ In contact with SeeedStudio support to get a replacement.
+ Found an alternate HBridge options.
+ Worked with David on ROS issues (described below)
+ Implemented and integrated apriltag detection in the brain app

**David**

+ With Naveen, fleshed out the design of the AT command interface.
+ Working on getting the brain logic to run in the background.
+ Did research into Android framework support for long-running jobs in the background.
+ Android offers four means of running jobs in the background: AsyncTasks, Threading, Services,
  and IntentServices. Each is suited for different use cases.
+ Since AsyncTasks are not well-suited to long-running jobs, and Threading requires manual
  lifecycle management, our choice is primarily between a Service and an IntentService.
+ A Service is not intended to run for a long time in the background, so Android will kill it unless
  it displays some kind of UI like a notification.
+ An IntentService is the intended way for jobs to run in the background, but it requires an event to
  fire periodically from some other service.
+ For our purposes, a Service is better suited - a notification is acceptable, but there is no
  obvious external event to trigger the IntentService.
+ Currently working on managing the service lifecycle and laying framework for the main loop.
+ The loop will consist of reading sensor data, updating movement state information, repathing if
  necessary, and issuing motor commands.

**Naveen**

+ Met with David to flesh out AT interface
+ ESP32 has a pattern recognition function for UART, may use that instead of AT which would require
  less work than repeatedly scanning for characters "A""T".
+ Decided to split esp32 operation into 3 parts.
+ The android task will be responsible for any interactions with the phone. Incoming messages will be
  distributed to the LIDAR or motor, depending on the command. There will also be an ACK bit sent back
  to the phone to ensure that the command was successfully processed by the ESP32.
+ The motor task will periodically check it's command buffer and process any commands, sending output
  if necessary to the buffer that will then be sent back to the phone
+ The LIDAR task will also check it's buffer and process any scan commands, sending output back to the buffer
  which will then be sent back to the android phone