import serial, socket, time

SERIAL_PORT = 0
TCP = 1

port = 3334

global handle

def calc_bcc(data, len):
    ret_val = ord(data[0])
    for i in range(len):
        print("ret_Val = ", ret_val, "xor = ", ord(data[i]))
        ret_val = ret_val ^ ord(data[i])
    return ret_val


def init(mod, com):
    global handle
    if(mod == SERIAL_PORT):
        handle = serial.Serial(com, baudrate = 300)
        print("Uart is initialized")
    if(mod == TCP):
        handle = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        handle.connect((com, port))
        print("Socket is initialized")

    #return handle

def write_tcp(content):
    handle.send(content.encode())

def write_serialPort(content):
    handle.write(content.encode())

functions[] = [write_serialPort, write_tcp]



class ososTest():

    def __init__(self, obis_code, device_num, com, port):
        self.obis_code = obis_code
        self.device_num = device_num
        self.com = com
        self.f = functions[0]

    def iec62051_portInit(port):
        if(port = 'TCP'):
            handle = serial.Serial(com, baudrate = 300)
            print("Uart is initialized")
        else:
            handle = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            handle.connect((com, port))
            print("Socket is initialized")




    def redirect_port(port):
        if(port = 'TCP')
            f = functions[1]
        else:
            f = functions[0]

    def send_handshake(self):
        osos_req = "/?" + str(self.device_num) + "!\r\n"
        self.handle.write(osos_req.encode())

    
    def send_readoutreq(self):
        readout_req = "\6"+ "000\r\n"
        self.handle.write(readout_req.encode())

        print(readout_req)

    def send_obisreq(self):
        obis_req = "\6"+ "001\r\n"
        self.handle.write(obis_req.encode())

    def send_readobis(self):
        send_data = "\1"+"R2"+"\2"+self.obis_code+"\3"
        bcc = calc_bcc(send_data, len(send_data))
        send_data = send_data + str(bcc)

        self.handle.write(send_data.encode())


    def start_testReadout(self):
        self.send_handshake()
        time.sleep(2)
        self.send_readoutreq()

    def start_testObis(self):
        self.send_handshake()
        time.sleep(2)
        self.send_obisreq()
        self.send_readobis()


