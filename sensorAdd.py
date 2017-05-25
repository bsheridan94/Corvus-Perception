import sys
import logging
import rds_config
import pymysql
import time
#rds settings
rds_host  = rds_config.db_instance_endpoint
name = rds_config.db_username
password = rds_config.db_password
db_name = rds_config.db_name


logger = logging.getLogger()
logger.setLevel(logging.INFO)

try:
    conn = pymysql.connect(rds_host, user=name, passwd=password, db=db_name, connect_timeout=5)
except:
    logger.error("ERROR: Unexpected error: Could not connect to MySql instance.")
    sys.exit()

logger.info("SUCCESS: Connection to RDS mysql instance succeeded")

def handler(node_id, TempReading, Humidity, Lux, rssi):

	st = time.strftime('%Y-%m-%d %H:%M:%S')
	
    with conn.cursor() as cur:
		SQL0 = "INSERT INTO system_health (time, node_id) VALUES ('%s', '%d')"
		cur.execute(SQL0, node_id)
        conn.commit()
		
		SQL_sel = "SELECT max(id) FROM corvus.system_health"
		cur.execute(SQL_sel)
        for row in cur:
			node_row_id = row
	
        SQL1 = "INSERT INTO temperature (n_id, TempReading, Humidity, Lux) VALUES ('%d', '%f', '%f', '%f')"
		cur.execute(SQL1, node_row_id, TempReading, Humidity, Lux)
        conn.commit()
		SQL2 = "INSERT INTO location (n_id, location) VALUES ('%d', '%d')"
		cur.execute(SQL2, node_row_id, rssi)
        conn.commit()

    return "Executed Command"