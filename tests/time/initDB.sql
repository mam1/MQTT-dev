
DROP DATABASE tq;
CREATE DATABASE tq;
USE tq;

CREATE TABLE Schedules (
    scheduleID MEDIUMINT NOT NULL AUTO_INCREMENT,
    name varchar(100) NULL,
    description varchar(100) NULL,
    PRIMARY KEY (scheduleID)
);

CREATE TABLE Transitions (
    transitionID MEDIUMINT NOT NULL AUTO_INCREMENT,
    scheduleID MEDIUMINT NULL,    
    offset int NOT NULL,
    state enum('on','off'),
    PRIMARY KEY (transitionID),
    FOREIGN KEY (scheduleID) REFERENCES Schedules(scheduleID)
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
