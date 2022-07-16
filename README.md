# DataAcquisitionSystem
Data acquisition system which it's responsible for interfacing with different sensors on our formula student car which participate in formula student uk competition https://www.imeche.org/events/formula-student and collects data from it and can provide essential information to the dashboard through can bus or to data logging or sending these data outside to monitor performance of the car on the track

Importance of data acquisition system:
Validate MATLAB model of our formula car with real collected data
Provide us with Euler angles and acceleration in 3D which can be used later to improve our mechanical design
Provide us with acceleration in 3D  
Provide us with RPM of the four wheels which can be used to calculate either car speed or gear
Provide us with the distance that suspension travelled
Provide us with angle that steering wheel had made it

Data acquisition system deals with:
IMU( Inertial measurement unit):                                                                                                       
This sensor consists of magnetometer and accelerometer and gyroscope ,also contain fusion mode that  fuses the reading from these sensors and provide us with Euler angles and  lateral acceleration
Proximity sensor:
Measure frequency of  moving wheels through input capture unit
Encoder:
Measures the rotational  angle of the  steering wheel 
Linear potentiometer  
Measures the distance that done by  suspension system by mapping voltage to distance

Code responisible for:
-Implementing the driver for IMU (Inertial Measurement Unit) BNO055 from Bosch
-Deals with CAN communication to transmit sensors readings through the bus
-Implement the whole project as tasks using FreeRTOS
-Deals with NUCLEO-F446RE
-Implement mechanism to detect if one of the sensors is cut off or failed and alerting the driver through the dashboard.
-Interfacing with different sensors on our formula car: 
1. Proximity sensor 
2. Travel sensor 
3. Encoder  
