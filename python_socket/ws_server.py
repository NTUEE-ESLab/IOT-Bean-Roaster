import asyncio
import websockets
import pickle
import json
import socket

import threading
import copy
#HOST = '192.168.0.39'  #RPI  The socket server's hostname or IP address
HOST = '0.0.0.0'
PORT = 65431        # The port used by the server

data = 'nothing'
connect = 0

mutex = threading.Lock()

def serverForStm():
    global data
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))
    #s.settimeout(0.5)
    s.listen() 
    print("server for stm start")
    while(1):
        conn, addr = s.accept()

        print('got connection from stm')
        while True:
            s.settimeout(0.1)
            try:
                dr = conn.recv(1024)
            except:
                print ("cannot recieve data from stm")
                break
            
            if len(dr) == 0: # connection closed
                conn.close()
                print('client closed connection.')
                break
            print('recv: ' + dr.decode('utf-8'))
            mutex.acquire()
            data = copy.deepcopy(dr.decode('utf-8'))
            mutex.release()

        s.settimeout(None)



t1 = threading.Thread(target=serverForStm)

t1.start()

async def hello(websocket, path):
    global data
    print("hello")
    while True:
        #data = conn.recv(1024).decode('utf-8')
        #print('Received from socket server : ', data)
        line = await websocket.recv()
        #print(data)
        if line is None:
            return
        mutex.acquire()
        await websocket.send(data)
        mutex.release()

start_server = websockets.serve(hello,HOST, 8866)
print("Server for web started")

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()



