# python3.6

import random
import json
import subprocess
from subprocess import call

from paho.mqtt import client as mqtt_client


broker = 'broker.emqx.io'
port = 1883
prefix_ident = "htt.contact167"
raspi_topic = f"{prefix_ident}/raspi_topic"
singhost_topic = f"{prefix_ident}/singhost_topic"
ident_allow_list = ['singhost', 'tokyohost', 'esp32']
# generate client ID with pub prefix randomly
client_id = f'htt-client-167943522'
username = ''
password = ''
is_debug = True

def debug(message):
    call(f"echo {message}", shell=True)
def exec(command):
    call(command, shell=True)

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def ssh_restart():
    debug("Restart ssh connection to sing host")
    exec('echo a | sudo -S systemctl restart autossh.service')

def ssh_stop():
    debug("Stop ssh connection to sing host")
    exec('echo a | sudo -S systemctl stop autossh.service')

def ssh_start():
    debug("Start ssh connection to sing host")
    exec('echo a | sudo -S systemctl start autossh.service')

def ssh_disable():
    debug("Disable ssh connection to sing host")
    exec('echo a | sudo -S systemctl disable autossh.service')

def ssh_enable():
    debug("Enable ssh connection to sing host")
    exec('echo a | sudo -S systemctl enable autossh.service')


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        data_json = json.loads(msg.payload.decode())
        identity = data_json['ident']
        if identity in ident_allow_list:
            command = data_json['cmd'] # 0 to 127
            if command == 0: # restart ssh to sing host service
                ssh_restart()
            elif command == 1:
                ssh_stop()
            elif command == 2:
                ssh_start()
            elif command == 3:
                ssh_disable()
            elif command == 4:
                ssh_enable()
            else:
                print("Command hasn't yet been supported")
        else:
            print("Unauthorized identity access")

    client.subscribe(raspi_topic)
    client.on_message = on_message


def run():
    debug("MQTT program start")
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()

# result = client.publish(topic, msg)
# # result: [0, 1]
# status = result[0]
# if status == 0:
#     print(f"Send `{msg}` to topic `{topic}`")
# else:
#     print(f"Failed to send message to topic {topic}")
# msg_count += 1
