
DROP DATABASE tq;
CREATE DATABASE tq;
USE tq;

CREATE TABLE Schedules (
    scheduleID MEDIUMINT NOT NULL AUTO_INCREMENT,
    name varchar(100) NULL,
    description varchar(100) NULL,
    minute_offset int NOT NULL,
    s_day int  NULL,
    s_hour int  NULL,
    s_minute int  NULL,
    state enum('on','off'),
    PRIMARY KEY (scheduleID)
); 

CREATE TABLE Sensors (
    sensorID MEDIUMINT NOT NULL AUTO_INCREMENT,
    name varchar(100) NULL,
    description varchar(100) NULL, 
    value int NULL,
    readtime timestamp NULL,
    PRIMARY KEY (sensorID)
); 

CREATE TABLE Channels (
    channelID MEDIUMINT NOT NULL AUTO_INCREMENT,
    name varchar(100) NULL,
    description varchar(100) NULL,
    scheduleID MEDIUMINT NULL,
    sensorID MEDIUMINT NULL,
    enabled enum('no','yes'),
    state enum('on','off'),
    PRIMARY KEY (channelID),
    FOREIGN KEY (scheduleID) REFERENCES Schedules(scheduleID),   
    FOREIGN KEY (sensorID) REFERENCES Sensors(sensorID) 
); 

