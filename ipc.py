

try:
    os.mkfifo(filename)
except OSError as e:
    print ("Failed to create FIFO: %s" % e)
else:
    fifo = open(filename, 'w')
    # write stuff to fifo
    print >> fifo, "hello"
    fifo.close()
    os.remove(filename)
    os.rmdir(tmpdir)
