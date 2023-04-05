import json
import time
import os, tempfile

# subprocess ipc pipe for read by cpp
tmpdir = tempfile.mkdtemp()
filename = os.path.join(tmpdir, 'myfifo')
print(filename)

class Sender:
    foo: str
    
    def __init__(self):
        foo = "I am sender prototype"


    def openFifo(filename):
        os.mkfifo(filename)
        fifo = open(filename, 'w')
        fifo.write("some testing data\n")

    def readFifo(filename):
        fifo = open(filename, 'r')
        while True:
            data = fifo.read()
            if len(data) == 0:
                break
            print('Received:', data)

    def exampleSender():
        try:
            # p1 = Process(target=openFifo, args=(filename,))
            # p1.start()

            # p2 = Process(target=readFifo, args=(filename,))
            # p2.start()

            os.mkfifo(filename)
            fifo = open(filename, 'w')
            fifo.write("some testing data\n")

        except OSError as e:
            print ("Failed to create FIFO: %s" % e)
        else:
            fifo = open(filename, 'w')
            # write stuff to fifo
            print >> fifo, "hello"
            fifo.close()
            os.remove(filename)
            os.rmdir(tmpdir)

