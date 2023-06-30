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
bmc_shell_rec = "0hPDq4RAxSc3xp"
esp_receive_topic = "FL4LYZOfQCVEpo"
esp_send_topic = "Klcgd1cSk7cvkQ"
# generate client ID with pub prefix randomly
client_id = 'WXhMjazOJ4eCBR' # default
previous_dir = ""
current_dir = "/"

shell_enabled = True

is_attach = True
filename = ""
mqtt_connect_status = False
output_received = False
send_id = 0
sudo_pass = ""
is_esp = True

# shell id 0 is for mc shell and 1 for raspi shell
macro_list = [
    ["wd_wake", "cd /home/pi/workspace/init_script/window_script && ./wd_wake.sh", 1],
    ["wd_hibernate", "cd /home/pi/workspace/init_script/window_script && ./wd_hibernate.sh", 1],
    ["wd_sleep", "cd /home/pi/workspace/init_script/window_script && ./wd_sleep.sh", 1],
    ["ssh_enable", "sudo systemctl restart autossh.service", 1],
    ["sd", "sudo shutdown -h now", 1]
]

def get_random_string(length):
    letters = string.ascii_letters + string.digits
    result_str = ''.join(random.choice(letters) for i in range(length))
    return result_str

def exec(command):
    call(command, shell=True)

def mqtt_command_inject(client, des_topic, byte_data, send_id):
    send_id_arr = [send_id]
    encode_data = [x + 128 for x in byte_data]
    shell_header_byte = []
    shell_header_byte.extend(ord(num) for num in shell_header)
    send_data = shell_header_byte + send_id_arr + encode_data
    send_data = bytearray(send_data)
    client.publish(des_topic, send_data)

def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        global current_dir, previous_dir
        global output_received
        global send_id
        # print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
        if msg.topic == receive_topic:
            buffer_data = msg.payload
            if buffer_data[0] == send_id:
                output_message = buffer_data[1:].decode('utf-8')
                cd_string = f"cd: {current_dir}"
                if cd_string in output_message:
                    current_dir = previous_dir
                print(output_message)
                output_received = True
        elif msg.topic == esp_receive_topic:
            output_received = True
            print("rsp -> " + msg.payload.decode())
        else:
            print("Invalid topic")
    client.subscribe(receive_topic)
    client.subscribe(esp_receive_topic)
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
                # mqtt_command_inject(client, raw_data)
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

def handle_input_cmd(command_input):
    global previous_dir
    global current_dir
    global sudo_pass
    previous_dir = current_dir
    if command_input.startswith("cd "):
        des_dir = command_input[3:]
        if "&&" in command_input:
            des_dir = command_input.split("&&")[0][3:].replace(" ","")
        else:
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
    elif command_input.startswith("sudo "):
        if sudo_pass == "":
            sudo_pass = input("Please input user password:")
        sudo_cmd = command_input[5:]
        command_input = f"echo {sudo_pass} | sudo -S " + sudo_cmd

    init_cmd = "cd " + current_dir
    str_cmd = "#!/bin/bash\n" + 'eval "$(cat /home/pi/.bashrc | tail -n +10)"\n' + init_cmd + "\n" + command_input
    return str_cmd

def handle_mc_cmd(client, command_input):
    cmd_arr = command_input.split(" ")
    if len(cmd_arr) < 2:
        print("Command not enough params")
        return False
    netfn = cmd_arr[0]
    action = cmd_arr[1]
    if netfn == "mc":
        if action == "on":
            client.publish(esp_send_topic, "1")
        elif action == "off":
            client.publish(esp_send_topic, "0")
        elif action == "get":
            client.publish(esp_send_topic, "2")
        elif action == "pin":
            option = cmd_arr[2]
            if option == "set":
                p_mode = cmd_arr[3]
                pin_num = cmd_arr[4]
                if p_mode == "output":
                    client.publish(esp_send_topic, f"3MO{pin_num}")
                elif p_mode == "input":
                    print("something")
                else:
                    print("Unknown pin mode: " + p_mode)
                    return False
            elif option == "level":
                pin_state = cmd_arr[3]
                pin_num = cmd_arr[4]
                if pin_state == "high":
                    client.publish(esp_send_topic, f"3SE{pin_num}")
                elif pin_state == "low":
                    client.publish(esp_send_topic, f"3SD{pin_num}")
                elif pin_state == "pushl":
                    client.publish(esp_send_topic, f"3SP{pin_num}")
                else:
                    print("Unknown pin state: " + pin_state)
                    return False
            else:
                print("Unknown option: " + option)
                return False
        else:
            print("Unknown action: " + action)
            return False
    elif netfn == "shell":
        if action == "enable":
            client.publish(send_topic, "Jd3RK1VllBZatc1")
        elif action == "disable":
            client.publish(send_topic, "Jd3RK1VllBZatc0")
        else:
            print("Unknown action: " + action)
            return False
    else:
        print("Unknowk netfn: " + netfn)
        return False
    return True


def run():
    global filename
    global is_attach
    global output_received
    global send_id, receive_id
    global client_id
    global current_dir
    global is_esp
    global broker, port, send_topic, receive_topic, enable_header, shell_header
    client_id = get_random_string(14)
    print("Program start")
    parser = optparse.OptionParser()
    parser.add_option("-a", "--attach",
                      type = 'string',
                      dest = "is_attach",
                      default = "directly",
                      help = "attach the console")
    parser.add_option("-f", "--file", dest = 'filename',
                      type = 'string', default = "",
                      help = 'specify a script file')
    parser.add_option("-c", "--config",
                     type = 'string',
                     dest = "config_file",
                     help = "specify config file")
    (options, args) = parser.parse_args()
    main_topic = ""

    config_file = open(options.config_file)
    config_data = json.load(config_file)

    broker = config_data['broker']
    port = config_data['port']
    send_topic = config_data['receive_topic']
    receive_topic = config_data['rsp_topic']
    enable_header = config_data['enable_header']
    shell_header = config_data['shell_header']

    if options.is_attach == "mc":
        # print("yes")
        is_esp = True
        is_attach = True
        main_topic = bmc_shell_rec
    elif options.is_attach == "directly":
        is_esp = False
        is_attach = False
        main_topic = send_topic
    else:
        print("unknown option attach option")
        exit(0)
    # if options.filename == "":
    #     # print("string empty")
    #     filename = ""
    # else:
    #     # print("filename: ", options.filename)
    #     filename = options.filename
    #     is_attach = False
    client = connect_mqtt()
    subscribe(client)
    # if is_attach:
    #     client.loop_start()
    # else:
    #     client.loop_forever()
    # print("here")
    client.loop_start()
    while True:
        if is_esp:
            command_input = input("mc <-: ")
            shell_id = 0
        else:
            main_topic = send_topic
            command_input = input(f"$ ({current_dir})\n-> ")
            shell_id = 1
        if mqtt_connect_status:
            if command_input == "exit":
                client.loop_stop()
                client.disconnect()
                break
            elif command_input == "switch":
                is_esp = not(is_esp)
                continue

            # check pre-define macro for each function shell
            for macro in macro_list:
                if macro[2] == shell_id:
                    if command_input == macro[0]:
                        command_input = macro[1]

            if len(command_input) != 0:
                if is_esp:
                    rc = handle_mc_cmd(client, command_input)
                    if rc:
                        count_timeout = 10
                        output_received = False
                        while True:
                            if count_timeout == 0 or output_received == True:
                                time.sleep(0.1)
                                break
                            count_timeout = count_timeout - 1
                            time.sleep(0.5)
                else:
                    str_cmd = handle_input_cmd(command_input)
                    # print("test: ", str_cmd)
                    cmd_arr = []
                    cmd_arr.extend(ord(num) for num in str_cmd)
                    send_id = send_id + 1
                    if send_id > 250:
                        send_id = 0
                    mqtt_command_inject(client, main_topic, cmd_arr, send_id)
                    count_timeout = 10
                    output_received = False
                    while True:
                        if count_timeout == 0 or output_received == True:
                            time.sleep(0.1)
                            break
                        count_timeout = count_timeout - 1
                        time.sleep(0.5)
        else:
            print("MQTT isn't connected, shell inject won't work")
        

if __name__ == '__main__':
    run()
