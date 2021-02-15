
DROP DATABASE tokenTest;
CREATE DATABASE tokenTest;
USE tokenTest;

CREATE TABLE Schedules (
    scheduleID INTEGER NOT NULL AUTO_INCREMENT,
    schedule_name varchar(100) NULL,
    schedule_description varchar(100) NULL,
    PRIMARY KEY (scheduleID)
);

CREATE TABLE Transitions (
    transitionID INTEGER NOT NULL AUTO_INCREMENT,
    scheduleID INTEGER NULL,    
    transition_offset int NOT NULL,
    transition_state enum('on','off'),
    PRIMARY KEY (transitionID),
    FOREIGN KEY (scheduleID) REFERENCES Schedules(scheduleID)
); 

CREATE TABLE Sensors (
    sensorID INTEGER NOT NULL AUTO_INCREMENT,
    sensor_name varchar(100) NULL,
    sensor_description varchar(100) NULL, 
    sensor_value int NULL,
    sensor_readtime timestamp NULL,
    PRIMARY KEY (sensorID)
); 

CREATE TABLE Channels (
    channelID INTEGER NOT NULL AUTO_INCREMENT,
    channel_name varchar(100) NULL,
    channel_description varchar(100) NULL,
    scheduleID INTEGER NULL,
    sensorID INTEGER NULL,
    channel_enabled enum('no','yes'),
    channel_state enum('on','off'),
    PRIMARY KEY (channelID),
    FOREIGN KEY (scheduleID) REFERENCES Schedules(scheduleID),   
    FOREIGN KEY (sensorID) REFERENCES Sensors(sensorID) 
);

CREATE TABLE TokenQ (
    tokenID INTEGER NOT NULL AUTO_INCREMENT,
    token varchar(100) NOT NULL,
    PRIMARY KEY (tokenID)
);


DELIMITER $$ ;
CREATE OR REPLACE PROCEDURE Push (newtoken varchar(100))
BEGIN
DECLARE newtokenID INTEGER;
SET newtokenID = tokenID;
SELECT * FROM TokenQ ORDER BY tokenID DESC LIMIT 1;
INSERT INTO TokenQ(tokenID, token) 
SELECT * FROM TokenQ WHERE tokenID = (newtokenID + 1);
END;
DELIMITER ; $$


DELIMITER $$ ;
CREATE OR REPLACE PROCEDURE Pop ()
BEGIN
DECLARE newtokenID INTEGER;
SET newtokenID = tokenID;
SELECT * FROM TokenQ ORDER BY tokenID DESC LIMIT 1;
SELECT * FROM TokenQ WHERE newtokenID = tokenID;
DELETE FROM TokenQ WHERE newtokenID = tokenID;
END;
DELIMITER ; $$