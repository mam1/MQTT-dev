CREATE TABLE Schedules (
    scheduleID int NOT NULL,
    *name varchar(100) NULL,
    *description varchar(100) NULL,
    action
    



    PRIMARY KEY (scheduleID),
   
); 



CREATE TABLE Channels (
    channelID int NOT NULL,



    channelID int,

 	PRIMARY KEY (channelID),
    FOREIGN KEY (scheduleID) REFERENCES Schedules(scheduleID)


    PRIMARY KEY (OrderID),
    FOREIGN KEY (channelID) REFERENCES Channels(channelID)
); 







CREATE TABLE `test-schedule`.NewTable (
	Column1 varchar(100) NULL
)
ENGINE=InnoDB
DEFAULT CHARSET=utf8mb4
COLLATE=utf8mb4_general_ci;