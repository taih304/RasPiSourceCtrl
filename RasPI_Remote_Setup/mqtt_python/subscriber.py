# python3.6

import time
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
            debug("Connected to MQTT Broker!")
        else:
            debug(f"Failed to connect, return code {rc}")

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def rsp_command(client, ident, cmd, message):
    rsp_data = {}
    rsp_data['ident'] = 'raspi'
    rsp_data['rsp'] = cmd
    rsp_data['message'] = message
    temp_topic = f"{prefix_ident}/{ident}_topic"
    client.publish(temp_topic, json.dumps(rsp_data))

def ssh_restart(client, identity):
    debug("Restart ssh connection to sing host")
    exec('echo a | sudo -S systemctl restart autossh.service')
    rsp_command(client, identity, 0, "autossh service restarted")

def ssh_stop(client, identity):
    debug("Stop ssh connection to sing host")
    exec('echo a | sudo -S systemctl stop autossh.service')
    rsp_command(client, identity, 1, "autossh service stopped")

def ssh_start(client, identity):
    debug("Start ssh connection to sing host")
    exec('echo a | sudo -S systemctl start autossh.service')
    rsp_command(client, identity, 2, "autossh service started")

def ssh_disable(client, identity):
    debug("Disable ssh connection to sing host")
    exec('echo a | sudo -S systemctl disable autossh.service')
    rsp_command(client, identity, 3, "autossh service disabled")

def ssh_enable(client, identity):
    debug("Enable ssh connection to sing host")
    exec('echo a | sudo -S systemctl enable autossh.service')
    rsp_command(client, identity, 4, "autossh service enabled")

def reboot_raspi(client, identity):
    rsp_command(client, identity, 5, "Reboot RasberryPI")
    debug("Reboot RasberryPI")
    exec('echo a | sudo -S reboot')

def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        data_json = json.loads(msg.payload.decode())
        identity = data_json['ident']
        if identity in ident_allow_list:
            command = data_json['cmd'] # 0 to 127
            if command == 0: # restart ssh to sing host service
                ssh_restart(client, identity)
            elif command == 1:
                ssh_stop(client, identity)
            elif command == 2:
                ssh_start(client, identity)
            elif command == 3:
                ssh_disable(client, identity)
            elif command == 4:
                ssh_enable(client, identity)
            elif command == 5:
                reboot_raspi(client, identity)
            else:
                print("Command hasn't yet been supported")
        else:
            print("Unauthorized identity access")

    client.subscribe(raspi_topic)
    client.on_message = on_message


def run():
    debug("MQTT program start, wait 30s for network fully loaded and connect to mqtt")
    time.sleep(30)
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()

