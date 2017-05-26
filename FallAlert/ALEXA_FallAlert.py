
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

def close(fulfillment_state, message):
    response = {
        'dialogAction': {
            'type': 'Close',
            'fulfillmentState': fulfillment_state,
            'message': message
        }
    }
    
    return response
    
def dispatch(intent_request, session):

    logger.debug(intent_request)

    intent = intent_request['intent']
    intent_name = intent_request['intent']['name']
    
    # Dispatch to your bot's intent handlers
    if intent_name == 'AlertUser':
        return make_appointment(intent, session)
    else:
        raise ValueError("Invalid intent")

def make_appointment(intent, session):
    session_attributes = {}
    reprompt_text = None
    should_end_session = True
    
    with conn.cursor() as cur:
        cur.execute("UPDATE alert_status SET Status='2' WHERE AlertType='Health'")  
        conn.commit()

    speech_output = "Sending Assistance Request "

    return build_response(session_attributes, build_speechlet_response(
        intent['name'], speech_output, reprompt_text, should_end_session))

def build_speechlet_response(title, output, reprompt_text, should_end_session):
    return {
        'outputSpeech': {
            'type': 'PlainText',
            'text': output
        },
        'card': {
            'type': 'Simple',
            'title': "SessionSpeechlet - " + title,
            'content': "SessionSpeechlet - " + output
        },
        'reprompt': {
            'outputSpeech': {
                'type': 'PlainText',
                'text': reprompt_text
            }
        },
        'shouldEndSession': should_end_session
    }

def build_response(session_attributes, speechlet_response):
    return {
        'version': '1.0',
        'sessionAttributes': session_attributes,
        'response': speechlet_response
    }

def handler(event, context):
    
    
    return dispatch(event['request'], event['session'])


