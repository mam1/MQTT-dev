
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
    type varchar(100) NOT NULL,
    value integer NULL,
    PRIMARY KEY (tokenID)
);

CREATE TABLE commandQ (
    commandID INTEGER NOT NULL AUTO_INCREMENT,
    command varchar(100) NOT NULL,
    PRIMARY KEY (commandID)
);


CREATE TABLE KeyWords (
    keywordID INTEGER NOT NULL AUTO_INCREMENT,
    keyword varchar(25) NOT NULL,
    description varchar(100) NULL,
    keycode INTEGER NOT NULL,
    PRIMARY KEY (keywordID)
);

alter table KeyWords add UNIQUE(keyword);

CREATE UNIQUE INDEX index_keywords ON KeyWords (keyword);

INSERT INTO KeyWords (keyword,keycode) VALUES("edit", 1);

INSERT INTO KeyWords (keyword,keycode) VALUES("channel", 2);
INSERT INTO KeyWords (keyword,keycode) VALUES("channels", 2);
INSERT INTO KeyWords (keyword,keycode) VALUES("chan", 2);

INSERT INTO KeyWords (keyword,keycode) VALUES("schedule", 3);
INSERT INTO KeyWords (keyword,keycode) VALUES("sch", 3);

INSERT INTO KeyWords (keyword,keycode) VALUES("save", 4);
INSERT INTO KeyWords (keyword,keycode) VALUES("quit", 5);

INSERT INTO KeyWords (keyword,keycode) VALUES("create", 6);

INSERT INTO KeyWords (keyword,keycode) VALUES("delete", 8);

INSERT INTO KeyWords (keyword,keycode) VALUES("display", 9);


