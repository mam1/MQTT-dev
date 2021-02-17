
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
CREATE OR REPLACE PROCEDURE push (IN newtoken varchar(100))
    BEGIN
        #DECLARE newtokenID INTEGER;
        SELECT tokenID FROM TokenQ ORDER BY tokenID DESC LIMIT 1;
        INSERT INTO TokenQ(token) VALUES(newtoken);
    END;
DELIMITER ; $$


DELIMITER $$ ;
#CREATE OR REPLACE PROCEDURE pop (OUT token varchar(100))
CREATE OR REPLACE PROCEDURE pop (OUT token varchar(100))
    BEGIN
        #select * from TokenQ;
        #DECLARE toke varchar(100)default " "; 
        SELECT token INTO @toke FROM TokenQ WHERE 1 = 1 ORDER BY tokenID LIMIT 1;
        #SELECT * FROM TokenQ ORDER BY tokenID LIMIT 1;
        SELECT @toke;
        #SET toke = token;
        #SET token = "hi there";
        SELECT CONCAT("HELLO"," ",@toke);
    END;
DELIMITER ; $$

CALL push("test1");
CALL push("test2");
CALL push("test333");
CALL push("test4");
CALL push("ttttt");





