SELECT * INTO OUTFILE '/tmp/your_table.csv' FIELDS TERMINATED BY ',' ENCLOSED BY '"' LINES TERMINATED BY '\n' FROM XYZ;


SELECT JSON_ARRAYAGG(JSON_OBJECT(
    'column1', column1,
    'column2', column2,
    ...
)) AS json_result
FROM your_table;



LOAD DATA INFILE '/tmp/remoteData.csv'
INTO TABLE users
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 LINES;  -- if your CSV has a header row
