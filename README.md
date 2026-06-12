# ESP32 Autonomous Folkrace Car

## Overview

This project presents the design and implementation of an autonomous folkrace vehicle developed for racing competitions involving dynamic environments and obstacle avoidance. The vehicle is capable of navigating a circular track, detecting and avoiding obstacles, adapting its speed based on surrounding conditions, and making real-time steering decisions using multiple ultrasonic sensors.

The system is built around an ESP32 microcontroller and integrates sensing, motor control, wireless communication, and decision-making algorithms to achieve autonomous behavior.

---

## Project Objectives

- Complete as many laps as possible within the competition time limit.
- Navigate a circular racing track autonomously.
- Detect and avoid static or dynamic obstacles.
- Maintain stable movement while adapting to changing track conditions.
- Implement intelligent steering and speed control strategies.

---

## Key Features

- Autonomous navigation using ultrasonic sensors.
- Real-time obstacle detection and avoidance.
- Dynamic speed adjustment based on front obstacle distance.
- Curve handling using side distance measurements.
- Differential steering control for smooth trajectory correction.
- Bluetooth communication for race start and stop commands.
- Modular software design for easy modification and expansion.

---

## Hardware Components

| Component | Description |
|-----------|-------------|
| ESP32 | Main microcontroller |
| DC Gear Motors | Vehicle propulsion |
| Motor Driver | Controls motor direction and speed |
| Ultrasonic Sensors | Distance measurement and obstacle detection |
| Bluetooth Module (ESP32 Built-in) | Remote start and stop functionality |
| Chassis and Wheels | Vehicle structure |

---

## Sensor Configuration

The vehicle utilizes five ultrasonic sensors strategically positioned around the car:

- Front Sensor: Detects obstacles directly ahead.
- Left Sensor: Assists with curve navigation and obstacle avoidance.
- Right Sensor: Assists with curve navigation and obstacle avoidance.
- Mid-Left Sensor: Improves environmental awareness.
- Mid-Right Sensor: Improves environmental awareness.

---

## System Behavior

### Normal Driving

The vehicle moves forward while continuously monitoring the surrounding environment.

### Speed Control

The distance measured by the front sensor determines the vehicle speed:

- Safe distance → Normal speed.
- Moderate distance → Reduced speed.
- Critical distance → Stop or initiate avoidance maneuver.

### Dynamic Steering

The vehicle compares distance measurements from the left and right sensors:

- Greater distance on the left side → Slight left steering adjustment.
- Greater distance on the right side → Slight right steering adjustment.
- Balanced readings → Maintain current heading.

This approach allows the car to navigate curves smoothly without abrupt steering actions.

### Obstacle Avoidance

When an obstacle is detected within a predefined threshold, the vehicle evaluates alternative paths and adjusts its trajectory accordingly.

---

## Software Architecture

The control software consists of the following functional modules:

- Sensor Acquisition Module
- Distance Processing Module
- Speed Control Module
- Steering Decision Module
- Obstacle Avoidance Module
- Bluetooth Communication Module
- Motor Control Module

---

## Development Tools

- Programming Language: Embedded C / Arduino Framework
- Development Environment: Arduino IDE
- Target Platform: ESP32

---

## Results

The implemented system successfully demonstrated:

- Autonomous lap completion.
- Real-time response to environmental changes.
- Stable curve handling behavior.
- Obstacle detection and avoidance capabilities.
- Wireless control integration through Bluetooth communication.

---

## Future Improvements

Potential enhancements include:

- Integration of Time-of-Flight (ToF) sensors for improved accuracy.
- Closed-loop speed control using wheel encoders.
- CAN communication support for automotive-oriented architectures.
- Migration to an RTOS-based software structure.
- Sensor fusion techniques for enhanced navigation performance.

---

## Skills Demonstrated

- Embedded C Programming
- ESP32 Development
- Real-Time Decision Making
- Sensor Integration
- Motor Control
- Bluetooth Communication
- Autonomous Navigation
- Embedded System Design
- Problem Solving and Debugging

---

## Author

**Alaa Salaheldein**

Electronics and Embedded Systems Engineer with interests in automotive embedded systems, autonomous vehicles, and intelligent control applications.
