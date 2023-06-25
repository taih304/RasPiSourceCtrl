# python3.6

import time
import random
import json
import subprocess
from subprocess import call
import os
import serial
import sys

from paho.mqtt import client as mqtt_client

config_file = open("/home/pi/workspace/control_service/.pi_config")

config_data = json.load(config_file)

broker = config_data['broker']
port = config_data['port']
receive_topic = config_data['receive_topic']
rsp_topic = config_data['rsp_topic']
rsp_topic1 = "FL4LYZOfQCVEpo"
enable_header = config_data['enable_header']
shell_header = config_data['shell_header']
# generate client ID with pub prefix randomly
client_id = config_data['client_id']

config_file.close()

# is_debug = True
is_debug = False
shell_enabled = False
serial_enabled = False
ser = ""

def debug(message):
    if is_debug:
        call(f"echo {message}", shell=True)
def exec(command):
    call(command, shell=True)

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            debug("Connected to MQTT Broker!")
        else:
            debug(f"Failed to connect, return code {rc}")

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def data_handler(client, data_arr):
    global shell_enabled, serial_enabled
    check_header = data_arr[0:14]
    header_str = ''.join(chr(i) for i in check_header)
    if header_str == enable_header:
        if data_arr[14] == 49:
            shell_enabled = True
            client.publish(rsp_topic1, "Service enabled")
        elif data_arr[14] == 48:
            shell_enabled = False
            client.publish(rsp_topic1, "Service disabled")
        else:
            print("Unknown active command")
    elif header_str == shell_header:
        if shell_enabled == True or serial_enabled == True:
            receive_id_str = ''.join(chr(data_arr[14]))
            raw_data = data_arr[15:]

            decode_data = [x - 128 for x in raw_data]

            str_data = ''.join(chr(i) for i in decode_data)
            # print(str_data)
            f = open("/tmp/tmp_script.sh", "w")
            f.write(str_data)
            f.close()
            exec("chmod +x /tmp/tmp_script.sh")
            cmd = ['/tmp/tmp_script.sh']
            proc = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            o, e = proc.communicate()
            output_str = o.decode('utf-8')
            if output_str == "":
                output_str = e.decode('utf-8')
            exec("rm -fr /tmp/tmp_script.sh")
            # print(output_str)
            final_output_str = receive_id_str + output_str
            if serial_enabled:
                # print(final_output_str)
                output_str_len = len(final_output_str)
                print(output_str_len)
                header_arr = b"~&" + output_str_len.to_bytes(2, sys.byteorder)
                packet_arr = header_arr + final_output_str.encode("utf-8")
                # final_len = len(packet_arr)
                # if final_len > 200:
                #     ptr = 0
                #     while ptr + 200 < final_len:
                #         ser.write(packet_arr[ptr:ptr+200])
                #         print(packet_arr[ptr:ptr+200])
                #         ptr = ptr + 200
                #     time.sleep(0.1)
                #     if ptr != final_len: # only happen if ptr > final_len, write the last segment
                #         ser.write(packet_arr[ptr:final_len])
                #         print(packet_arr[ptr:final_len])
                # else:
                #     ser.write(packet_arr)
                ser.write(packet_arr)
                serial_enabled = False
            else:
                client.publish(rsp_topic, final_output_str)
    else:
        print("Unknown header str")


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        global shell_enabled
        # print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        if msg.topic == receive_topic:
            msg_arr = msg.payload
            data_handler(client, msg_arr)
        else:
            print("Invalid topic")


    client.subscribe(receive_topic)
    client.on_message = on_message


def run():
    global ser, serial_enabled
    debug("MQTT program start, wait 30s for network fully loaded and connect to mqtt")
    time.sleep(20)
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()
    # client.loop_start()

    # receive serial data
   



if __name__ == '__main__':
    run()

