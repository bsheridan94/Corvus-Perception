import sys
import logging
import rds_config
import pymysql
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


    """
    This function fetches content from mysql RDS instance
    """

with conn.cursor() as cur:
	cur.execute("UPDATE alert_status SET Status='1' WHERE AlertType='Health'")  
	conn.commit()