import json
import time
import os, tempfile

# Subprocess ipc pipe for read by cpp
# This class function is send data to pipe and write onto it
# initial configuration made in constructor
# then open the fifo based pipe
# and after readers are taken in pipe like outside cpp,python etc.
# 
# this class also receiver compatible with private methods
class Sender:
    foo: str
    
    # pipe creation logic in constructor
    def __init__(self, pipename):
        self.pipename = pipename
        self.tmpdir = tempfile.mkdtemp()
        self.path2fifo = os.path.join(self.tmpdir, self.pipename)
        print( "pipe is configuration at : " + self.path2fifo)


    def __del__(self):
        self.fifo.close()
        os.remove(self.path2fifo)
        os.rmdir(self.tmpdir)

    def openFifo(self):
        try:

            os.mkfifo(self.path2fifo)
            print("pipe opening, please add reader to it :" + self.path2fifo)
            self.fifo = open(self.path2fifo, 'w')
            # self.fifo.write("some testing data\n")
            print("pipe opened at :" + self.path2fifo)


        except OSError as e:
            print ("Failed to create FIFO: %s" % e)
            raise Exception("Fifo does exist")


    def _readFifo(self):
        readerFifo = open(self.path2fifo, 'r')
        while True:
            data = readerFifo.read()
            if len(data) == 0:
                break
            print('Received:', data)

    def sendData(self, data):
        self.fifo.write(data)

    def sendDataWithFlushBuffer(self, data):
        self.fifo.write(data+"\n")
        self.fifo.flush()

    def flushBuffer(self):
        self.fifo.flush()

    def pipeLocation(self):
        return self.path2f55ifo
    def _setPipeDirectory(self, directoryPath):
        self.tmpdir = directoryPath
        self.path2fifo = os.path.join(self.tmpdir, self.pipename)
        print("setting new pipe location to " + self.path2fifo)

    def writePipeFilePath2AppSetting(self):
        file = "appsetting.json"
        if(os.path.exists(file)):
            appSetting = open(file,'w+')
            appSetting.seek(0)
            jsonData=json.load(appSetting)
            jsonData['pipePath'] = self.path2fifo
            appSetting.seek(0)
            json.dump(jsonData, appSetting)
        else:
            appSetting = open("appsetting.json", 'w+')
            appSetting.write('{}')
            appSetting.seek(0)
            jsonData=json.loads(appSetting.read())
            jsonData['pipePath'] = self.path2fifo
            appSetting.seek(0)

            json.dump(jsonData, appSetting)

    def _setPipeLocationWithPipename(self, path):
        self.path2fifo = path
