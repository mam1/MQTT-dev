DROP DATABASE tq;
CREATE DATABASE tq;
USE tq;

CREATE TABLE Schedules (
    scheduleID MEDIUMINT NOT NULL AUTO_INCREMENT,
    schedule_name varchar(100) NULL,
    schedule_description varchar(100) NULL,
    PRIMARY KEY (scheduleID)
);

CREATE TABLE Transitions (
    transitionID MEDIUMINT NOT NULL AUTO_INCREMENT,
    scheduleID MEDIUMINT NULL,    
    transition_offset int NOT NULL,
    transition_state enum('on','off'),
    PRIMARY KEY (transitionID),
    FOREIGN KEY (scheduleID) REFERENCES Schedules(scheduleID)
); 

CREATE TABLE Sensors (
    sensorID MEDIUMINT NOT NULL AUTO_INCREMENT,
    sensor_name varchar(100) NULL,
    sensor_description varchar(100) NULL, 
    sensor_value int NULL,
    sensor_readtime timestamp NULL,
    PRIMARY KEY (sensorID)
); 

CREATE TABLE Channels (
    channelID MEDIUMINT NOT NULL AUTO_INCREMENT,
    channel_name varchar(100) NULL,
    channel_description varchar(100) NULL,
    scheduleID MEDIUMINT NULL,
    sensorID MEDIUMINT NULL,
    channel_enabled enum('no','yes'),
    channel_state enum('on','off'),
    PRIMARY KEY (channelID),
    FOREIGN KEY (scheduleID) REFERENCES Schedules(scheduleID),   
    FOREIGN KEY (sensorID) REFERENCES Sensors(sensorID) 
); 

