#coding=utf-8
#!/usr/bin/env python
import threading
import time

condition = threading.Condition()
money = 0

class Save(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        global condition, money
        while True:
            if condition.acquire():
                money += 10
                print "save money(%s): now money is %s" %(self.name, money)
                condition.notify()
            condition.release()
            time.sleep(1)

class Load(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        global condition, money
        while True:
            if condition.acquire():
                if money>0:
                    money -= 10
                    print "load money(%s): now money is %s" %(self.name, money)
                    condition.notify()
                else:
                    print "load money(%s): no money now" %(self.name)
                    condition.wait()
            condition.release()
            time.sleep(2)

if __name__ == "__main__":
    for s in range(0,2):
        s = Save()
        s.start()
    for l in range(0,4):
        l = Load()
        l.start()
